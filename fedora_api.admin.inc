<?php

/**
 * @file
 * Administrative page callbacks for the fedora api module.
 */

/**
 * Form builder to list and manage Fedora connections
 * @ingroup forms
 * 
 * @param array $form 
 */
function fedora_api_overview_connections($form) {
  
}

function fedora_api_form_connection($form, &$form_state, $edit = array()) {
  if (!isset($form_state['client'])) {
    $connection = is_object($edit) ? $edit : (object) $edit;
    $defaults = array(
      'name' => '',
      'machine_name' => '',
      'description' => '',
      'url' => '',
      'username' => '',
      'password' => '',
      'weight' => 0,
      
    );
    foreach ($defaults as $key => $value) {
      if (!isset($connection->$key)) {
        $connection->$key = $value;
      }
    }
    $form_state['connection'] = $connection;
  }
  else {
    $connection = $form_state['connection'];
  }
  
  // Check whether we need a deletion confirmation form
  if (isset($form_state['confirm_delete']) && isset($form_state['values']['cid'])) {
    return fedora_api_connection_confirm_delete($form, $form_state, $form_state['values']['cid']);
  }
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $connection->name,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $connection->machine_name,
    '#maxlength' => 21,
    '#machine_name' => array(
      # TODO: Implement fedora_api_connection_machine_name_load()
      '#exists' => 'fedora_api_connection_machine_name_load',
    ),
  );
  
  
}

function fedora_api_connection_confirm_delete($form, &$form_state, $cid) {
  
}