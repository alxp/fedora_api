<?php

require_once('fedora_api.raw.inc');
require_once('FedoraRestException.php');
/**
 * Wrapper for Fedora Commons API functions that don't operate on existing objects.
 */
class FedoraClient {
  
  public $connection;
  
  function __construct($connection = NULL) {
    if (empty($connection)) {
      $connection = new FedoraConnection();
    }
    
    $this->connection = $connection;
    $this->api = new FedoraAPI($this->connection);
  }
  
  /**
   * Ingest a new object, with either an FOXML file, an FOXML string or neither for a blank object.
   * @param string $foxml_file
   * @param string $foxml_string
   * @param array $options
   * @return string The PID of the newly created object. 
   */
  function ingest($foxml_file = NULL, $foxml_string = NULL, 
          $options = array('pid' => NULL, 'new' => NULL, 'label' => NULL, 'format' => NULL, 'encoding' => NULL,
                                                     'namespace' => NULL, 'ownerId' => NULL, 'logMessage' => NULL,)) {
    
    $response = $this->api->ingest($foxml_file, $foxml_string, !empty($options['pid']) ? $options['pid'] : NULL, 
            !empty($options['new']) ? $options['new'] : NULL, !empty($options['label']) ? $options['label'] : NULL,
            !empty($options['format']) ? $options['format'] : NULL, !empty($options['encoding']) ? $options['encoding'] : NULL,
            !empty($options['namespace']) ? $options['namespace'] : NULL, !empty($options['ownerId']) ? $options['ownerId'] : NULL, 
            !empty($options['logMessage']) ? $options['logMessage'] : NULL);
    
    if ($response->code != 201) {
      throw new FedoraRestException($response->error);
    }
    return $response->data;
  }
}