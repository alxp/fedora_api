<?php

include_once('fedora_api.item.inc');

class FedoraDatastream extends PropertyObject {
  public $parent = NULL;
  public $dsID = '';
  
  private $_dsLabel;
  private $_dsVersionID;
  private $_dsCreateDate;
  private $_dsState;
  private $_dsMIME;
  private $_dsFormatURI;
  private $_dsSize;
  private $_dsVersionable;
  private $_dsInfoType;
  private $_dsLocation;
  private $_dsLocatoinType;
  private $_dsChecksumType;
  private $_dsChecksum;
  function __construct($parent = NULL, $dsID = NULL) {
    $this->parent = $parent;
    $this->_dsID = $dsID;
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
  }
  
  protected function get_dsID() {
    return $this->_dsID;
  }
  
  protected function get_content() {

    $resp = $this->parent->client->api->getDatastreamDissemination($this->parent->pid, $this->_dsID);
    if ($resp->code != 200) {
      throw new FedoraRestException("Error getting datastream contents $dsID . $resp->error");
    }
    return $resp->data;
  }
  
  protected function set_content() {
    
  }
  
  protected function get_dsLabel() {
    return $this->_dsLabel;
  }
  
  protected function set_dsLabel($label) {
    $resp = $this->parent->client->api->modifyDatastream($this->parent->pid, $this->_dsID, NULL, NULL, NULL, $label);
    if ($resp->code != 200) {
      throw new FedoraRestException("Unable to set label, $resp->error");
    }
    $this->_dsLabel = $label;
  }
  
  protected function get_dsVersionID() {
    return $this->_dsVersionID;
  }
  
  protected function get_dsCreateDate() {
    return $this->_dsCreateDate;
  }
  
  protected function get_dsState() {
    return $this->_dsState;
  }
  
  protected function get_dsMIME() {
    return $this->_dsMIME;
  }
  
  protected function get_dsFormatURI() {
    return $this->_dsFormatURI;
  }
  
  protected function get_dsSize() {
    return $this->_dsSize;
  }
  
  protected function get_dsVersionable() {
    return $this->_dsVersionable;
  }
  
  protected function get_dsInfoType() {
    return $this->_dsInfoType;
  }
  
  protected function get_dsLocation() {
    return $this->_dsLocation;
  }
  
  protected function get_dsLocationType() {
    return $this->_dsLocationType;
  }
  
  protected function get_dsChecksumType() {
    return $this->_dsChecksumType;
  }
  
  protected function get_dsChecksum() {
    return $this->_dsChecksum;
  }
  
  private function _decodeDatastreamXML($xml) {
    $ds_info = new SimpleXMLElement($xml);
    $this->_dsLabel = (string) $ds_info->dsLabel;
    $this->_dsVersionID = (string) $ds_info->dsVersionID;
    $this->_dsCreateDate = new DateTime((string) $ds_info->createdDate);
    $this->_dsState = (string) $ds_info->dsState;
    $this->_dsMIME = (string) $ds_info->dsMIME;
    $this->_dsFormatURI = (string) $ds_info->dsFormatURI;
    $this->_dsSize = (int) (string) $ds_info->dsSize;
    $this->_dsVersionable = (boolean) (string) $ds_info->dsVersionable;
    $this->_dsInfoType = (string) $ds_info->dsInfoType;
    $this->_dsLocation = (string) $ds_info->dsLocation;
    $this->_dsLocationType = (string) $ds_info->dsLocationType;
    $this->_dsChecksumType = (string) $ds_info->dsChecksumType;
    $this->_dsChecksum = (string) $ds_info->dsChecksum;

  }

}