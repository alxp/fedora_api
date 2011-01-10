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
      $this->datastreams_list =& Fedora_Item::$instantiated_pids[$pid]->datastreams_list;
    }
    else { 
      $response = fedora_api_getObjectProfile($pid);
      
      

      if ($response->code == 200) {
	$this->objectProfile = new SimpleXMLElement($response->data);
	$this->datastreams = $this->get_datastreams_list_as_array();
      } else {
	$this->objectProfile = '';
	$this->datastreams = array();
      }
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
    $this->get_datastreams_list_as_SimpleXML();
    $ds_list = array();
    if ($this->datastreams_list != FALSE) {

      // This is a really annoying edge case: if there is only one
      // datastream, instead of returning it as an array, only
      // the single item will be returned directly. We have to
      // check for this.
      if (count($this->datastreams_list->datastreamDef) >= 2) {
        foreach ($this->datastreams_list->datastreamDef as $ds) {
          if (!is_object($ds)) {
            print_r($ds);
          }
          $ds_list[$ds->ID]['label'] = $ds->label;
          $ds_list[$ds->ID]['MIMEType'] = $ds->MIMEType;
          $ds_list[$ds->ID]['URL'] = $this->url() . '/'. $ds->ID . '/'. drupal_urlencode($ds->label);
        }
      }
      else {
        $ds = $this->datastreams_list->datastreamDef;
        $ds_list[$ds->ID]['label'] = $ds->label;
        $ds_list[$ds->ID]['MIMEType'] = $ds->MIMEType;
        $ds_list[$ds->ID]['URL'] = $this->url().'/'.$ds->ID.'/'.drupal_urlencode($ds->label);
      }
    }

    return $ds_list;
  }

}