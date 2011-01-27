<?php

include_once('fedora_api.item.inc');

class FedoraDatastream extends PropertyObject {
  public $parent = NULL;
  public $dsID = '';
  private $_construct = FALSE;
  
  function __construct($parent = NULL, $dsID = NULL) {
    $this->_construct = TRUE;
    $this->parent = $parent;
    $this->dsID = $dsID;
    if (!empty($parent) && !empty($dsID)) {
      $resp = $parent->client->api->getDatastream($parent->pid, $dsID);
      if ($resp->code == 200) {
        $this->_decodeDatastreamXML($resp->data);
      }
      else {
        throw new FedoraRestException("Unable to create datastream for $dsID. $resp->error.");
      }
    }
    else {
      throw new FedoraRestException("Unable to create datastream for $dsID. Empty FedoraItem or Datastream ID given..");
    }
    $this->_construct = FALSE;
  }
  
  protected function get_dsID() {
    return $this->dsID;
  }
  
  protected function get_content() {

    $resp = $this->parent->client->api->getDatastreamDissemination($this->parent->pid, $this->dsID);
    if ($resp->code != 200) {
      throw new FedoraRestException("Error getting datastream contents $dsID . $resp->error");
    }
    return $resp->data;
  }
  
  protected function set_content() {
    
  }
  
  protected function get_dsLabel() {
    return $this->dsLabel;
  }
  
  protected function set_dsLabel($label) {
    $resp = $this->parent->client->api->modifyDatastream($this->parent->pid, $this->dsID, NULL, NULL, NULL, $label);
    if ($resp->code != 200) {
      throw new FedoraRestException("Unable to set label, $resp->error");
    }
    $this->dsLabel = $label;
  }
  
  protected function get_dsVersionID() {
    return $this->dsVersionID;
  }
  
  protected function get_dsCreateDate() {
    return $this->dsCreateDate;
  }
  
  protected function get_dsState() {
    return $this->dsState;
  }
  
  protected function get_dsMIME() {
    return $this->dsMIME;
  }
  
  protected function get_dsFormatURI() {
    return $this->dsFormatURI;
  }
  
  protected function get_dsSize() {
    return $this->dsSize;
  }
  
  protected function get_dsVersionable() {
    return $this->dsVersionable;
  }
  
  protected function get_dsInfoType() {
    return $this->dsInfoType;
  }
  
  protected function get_dsLocation() {
    return $this->dsLocation;
  }
  
  protected function get_dsLocationType() {
    return $this->dsLocationType;
  }
  
  protected function get_dsChecksumType() {
    return $this->dsChecksumType;
  }
  
  protected function get_dsChecksum() {
    return $this->dsChecksum;
  }
  
  private function _decodeDatastreamXML($xml) {
    $ds_info = new SimpleXMLElement($xml);
    $this->_construct = TRUE;
    $this->dsLabel = (string) $ds_info->dsLabel;
    $this->dsVersionID = (string) $ds_info->dsVersionID;
    $this->dsCreateDate = new DateTime((string) $ds_info->createdDate);
    $this->dsState = (string) $ds_info->dsState;
    $this->dsMIME = (string) $ds_info->dsMIME;
    $this->dsFormatURI = (string) $ds_info->dsFormatURI;
    $this->dsSize = (int) (string) $ds_info->dsSize;
    $this->dsVersionable = (boolean) (string) $ds_info->dsVersionable;
    $this->dsInfoType = (string) $ds_info->dsInfoType;
    $this->dsLocation = (string) $ds_info->dsLocation;
    $this->dsLocationType = (string) $ds_info->dsLocationType;
    $this->dsChecksumType = (string) $ds_info->dsChecksumType;
    $this->dsChecksum = (string) $ds_info->dsChecksum;
    $this->_construct = FALSE;
  }
}