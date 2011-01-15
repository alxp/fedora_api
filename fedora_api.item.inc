<?php

/**
 * An wrapper for Fedora's raw API calls to present Fedora items as manageable objects.
 */
require_once('PropertyObject.inc');

class Fedora_Item extends PropertyObject {
  public $pid = NULL; // The $pid of the fedora object represented by an instance of this class.
  private $objectProfile = NULL;
  public $datastreams = NULL;
  public $connection = NULL;
  private static $instantiated_pids = array();
  
  
  /**
   * Create an object to represent an item in the Fedora repository.
   * Throws a SOAPException if the PID is not in the repository.
   *
   * @param string $pid
   * @param FedoraConnection $connection
   * @return Fedora_Item
   */
  function __construct($pid, $connection = NULL) {
    if ($this->connection) {
      
    }
    else {
      // Make a default connection using Drupal's variables.
    }
    $this->pid = $pid;
    if (isset(Fedora_Item::$instantiated_pids[$pid])) {
      $this->connection =& Fedora_Item::$instantiated_pids[$pid]->connection;
      $this->objectProfile =& Fedora_Item::$instantiated_pids[$pid]->objectProfile;
      $this->datastreams =& Fedora_Item::$instantiated_pids[$pid]->datastreams;
      $datastreams_doc =& Fedora_Item::$instantiated_pids[$pid]->datastreams_list;
    }
    else { 
      $this->refreshObjectProfile();
      Fedora_Item::$instantiated_pids[$pid]=&$this;
    }
  }
 
  
  /**
   * Returns an associative array of this object's datastreams. Results look like this:
   *
   *  'DC' =>
   *    array
   *      'label' => string 'Dublin Core Record for this object' (length=34)
   *      'MIMEType' => string 'text/xml' (length=8)
   *  'RELS-EXT' =>
   *    array
   *      'label' => string 'RDF Statements about this object' (length=32)
   *      'MIMEType' => string 'application/rdf+xml' (length=19)
   *
   * @return array
   */
  function get_datastreams_list_as_array() {
    
    $result = fedora_api_listDatastreams($this->pid);
    if ($result->code != 200) {
      return NULL;
    }
    $datastreams_doc = new SimpleXMLElement($result->data);
    $ds_list = array();
    if ($datastreams_doc != FALSE) {

      foreach ($datastreams_doc->datastream as $ds) {
        foreach($ds->attributes() as $field => $value) {
          switch ($field) {
            case 'dsid':
              $dsid = (string) $value;
              break;            
            case 'label':
              $dslabel = (string) $value;
              break;
            case 'mimeType':
              $dsmime = (string) $value;
              break;
          }
        }
        if (!empty($dsid) && !empty($dsmime) && $dslabel != NULL) {
          $ds_list[$dsid] = array();
          $ds_list[$dsid]['label'] = $dslabel;
          $ds_list[$dsid]['mimeType'] = $dsmime;
        }
      }
    }
    return $ds_list;
  }
  
  protected function get_label() {
    return (string) $this->objectProfile->objLabel;
  }
  
  protected function set_label($label) {
    $this->modifyObject($label);
  }
  
  protected function get_ownerId() {
    return  (string) $this->objectProfile->objOwnerId;
  }
  
  protected function set_ownerId($ownerId) {
    $this->modifyObject($this->label, $ownerId);
  }
  
  protected function get_state() {
    return (string) $this->objectProfile->objState;
  }
  
  protected function set_state($state) {
    $this->modifyObject(NULL, NULL, $state);

  }
  
  protected function get_pid() {
    return $this->pid;
  }
  
  protected function get_createDate() {
    return new DateTime((string) $this->objectProfile->objCreateDate);
  }
  
  protected function get_lastModDate() {
    return  new DateTime((string) $this->objectProfile->objLastModDate);
  }
  
  protected function get_models() {
    $models = array();
    foreach ($this->objectProfile->objModels->model as $model) {
      $models[] = (string) $model;
    }
    return $models;
  }
  
  private function refreshObjectProfile() {
    $response = fedora_api_getObjectProfile($this->pid);

    if ($response->code == 200) {
      $this->objectProfile = new SimpleXMLElement($response->data);
      $this->datastreams = $this->get_datastreams_list_as_array();
    } else {
      $this->objectProfile = '';
      $this->datastreams = array();
    }
  }
  
  /**
   * Wrapper for Fedora's modifyObject REST call. Updates the local objectProfile after each call.
   * @param type $label
   * @param type $ownerId
   * @param type $state One of 'A' for active, 'D' for deleted, or 'I' for inactive 
   * @param type $logMessage 
   */
  private function modifyObject($label = NULL, $ownerId = NULL, $state = NULL, $logMessage = NULL) {
    $response = fedora_api_modifyObject($this->pid, $label, $ownerId, $state, $logMessage);
    if ($response->code == 200) {
      $this->refreshObjectProfile();
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  
  static function createItem( ) {
    fedora_api_ingest();
  }
}
