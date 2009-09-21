<?php
// $Id$

/**
 * Ting search settings form.
 */
function ting_admin_settings_form($form_state) {
  $form = array();

  $form['ting_search_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Search service URL'),
    '#description' => t('URL to the Ting search webservice, e.g. http://didicas.dbc.dk/opensearch'),
    '#required' => TRUE,
    '#default_value' => variable_get('ting_search_url', ''),
  );
    $form['ting_scan_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Scan service URL'),
    '#description' => t('URL to the Ting scan webservice, e.g. http://didicas.dbc.dk/openscan'),
    '#required' => TRUE,
    '#default_value' => variable_get('ting_scan_url', ''),
  );
  $form['ting_spell_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Spell service URL'),
    '#description' => t('URL to the Ting spell webservice, e.g. http://didicas.dbc.dk/openspell'),
    '#required' => TRUE,
    '#default_value' => variable_get('ting_spell_url', ''),
  );
  $form['ting_recommendation_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Recommendation service URL'),
    '#description' => t('URL to the Ting recommendation webservice (Andre der har lånt...) , e.g. http://didicas.dbc.dk/openadhl'),
    '#required' => TRUE,
    '#default_value' => variable_get('ting_recommendation_server', ''),
  );

  return system_settings_form($form);
}

