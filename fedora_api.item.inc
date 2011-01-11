<?php

/**
 * An wrapper for Fedora's raw API calls to present Fedora items as manageable objects.
 */
class Fedora_Item {
  public $pid = NULL; // The $pid of the fedora object represented by an instance of this class.
  public $objectProfile = NULL;
  public $datastreams = NULL;
  
  private static $instantiated_pids = array();
  
  
  /**
   * Create an object to represent an item in the Fedora repository.
   * Throws a SOAPException if the PID is not in the repository.
   *
   * @param string $pid
   * @return Fedora_Item
   */
  function __construct($pid) {
    
    $this->pid = $pid;
    if (isset(Fedora_Item::$instantiated_pids[$pid])) {
      $this->objectProfile =& Fedora_Item::$instantiated_pids[$pid]->objectProfile;
      $this->datastreams =& Fedora_Item::$instantiated_pids[$pid]->datastreams;
      $datastreams_doc =& Fedora_Item::$instantiated_pids[$pid]->datastreams_list;
    }
    else { 
      $response = fedora_api_getObjectProfile($pid);
      
      

      if ($response->code == 200) {
        $this->objectProfile = new SimpleXMLElement($response->data);
        $this->datastreams = $this->get_datastreams_list_as_array();
        $this->parseObjectProfile($this->objectProfile);  
      } else {
        $this->objectProfile = '';
        $this->datastreams = array();
      }
      Fedora_Item::$instantiated_pids[$pid]=&$this;
    }
  }
  
  private function parseObjectProfile($object_profile) {
    $this->label = (string) $object_profile->objLabel;
    $this->ownerId = (string) $object_profile->objOwnerId;
    $this->models = array();
    foreach ($object_profile->objModels->model as $model) {
      $this->models[] = (string) $model;
    }
    $this->createDate = new DateTime((string) $object_profile->objCreateDate);
    $this->lastModDate = new DateTime((string) $object_profile->objLastModDate);
    $this->state = (string) $object_profile->objState;
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
}