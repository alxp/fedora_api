<?php

/**
 * Wrapper for Fedora's findObjects API function.
 */
function fedora_findObjects($terms = array(), $query = array()) {
  
  $resp = fedora_api_findObjects(NULL, "pid=islandora:demos" );
  //fedora_api_findObjects($terms, $query, $maxResults, $resultFormat, $displayFields)
  print_r ($resp);
  $results = new SimpleXMLElement($resp->data);
  foreach ($results->resultList->objectFields as $obj) {
    print_r((string) $obj->pid);
    print_r((string) $obj->title);
  }
  print_r($results);
}