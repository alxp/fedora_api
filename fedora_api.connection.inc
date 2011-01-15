<?php
/**
 * Represents a connection to a Fedora-Commons Repository using the REST API
 */
class FedoraConnection {
  public $url;
  public $username;
  public $password;
  
  function __construct($url, $username = NULL, $password = NULL) {
    $this->url = $url;
    $this->username = $username;
    $this->password = $password;
    
    
  }
  
  function getObject($pid) {
    return new Fedora_Item($pid, $this);
  }
}