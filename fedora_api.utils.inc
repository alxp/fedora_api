<?php

/**
 * Wrapper for Fedora's findObjects API function.
 * @param string $terms Search terms, e.g. "changeme:*"
 * @param string $query Detailed search query, e.g. "pid=demo:12"
 * @param int $max_results
 * @param array $display_fields 
 */
function fedora_findObjects($terms = NULL, $query = NULL, $max_results = 50, 
        $display_fields = array('pid','label', 'state', 'ownerId', 'cDate', 'mDate', 'dcmDate', 
            'title', 'creator', 'subject', 'description', 'publisher', 'contributor', 'date', 'type', 
            'format', 'identifier', 'source', 'language', 'relation', 'coverage', 'rights')) {
  // pid needs to be part of the displayFields so we can construct the results array.  
  if (!in_array('pid', $display_fields)) {
    $display_fields[] = 'pid';
  }
  $resp = fedora_api_findObjects($terms, $query, $max_results, 'xml', $display_fields);
  //fedora_api_findObjects($terms, $query, $maxResults, $resultFormat, $displayFields)
  //print_r ($resp);
  $results = new SimpleXMLElement($resp->data);
  $resultset = array();
  foreach ($results->resultList->objectFields as $obj) {
    $pid = (string) $obj->pid;
    $resultset[$pid] = array();
    
    foreach ($obj->children() as $field) {
      $field_name = $field->getName();
      if ($field_name != 'pid') {
        $resultset[$pid][$field_name] = (string) $field;
      }
    }
  }
  return $resultset;
}