<?php
// $Id$

/**
 * Ting search settings form.
 */
function ting_admin_settings_form($form_state) {
  $form = array();

  $form['ting_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Ting server URL'),
    '#description' => t('URL to the Ting search service, e.g. http://didicas.dbc.dk/opensearch'),
    '#required' => TRUE,
    '#default_value' => variable_get('ting_server', ''),
  );

  return system_settings_form($form);
}

