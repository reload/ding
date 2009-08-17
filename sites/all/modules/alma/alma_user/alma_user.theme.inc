<?php
// $Id$

/**
 * Preprocessor for the user status template.
 */
function template_preprocess_alma_user_status_block(&$variables) {
}

/**
 * Preprocessor for the alma_user_personal_info template.
 */
function template_preprocess_alma_user_personal_info(&$variables) {
  $variables['links'] = theme('item_list', array(
    l('Edit contact information', 'user/' . $variables['account']->uid . '/edit_contact_info'),
    l('Change PIN code', 'user/' . $variables['account']->uid . '/change_pin'),
  ));
}

/**
 * Theming of a records list.
 *
 * Usually a list of reservations, books, etc, coming from Alma.
 */
function theme_alma_user_record_list($records) {
  $items = array();

  foreach ($records as $rec) {
    // TODO: We need to get the name from Ting based on the record ID.
    $items[] = $rec['record_id'];
  }

  return theme('item_list', $items, NULL, 'ul', array('class' => 'record-list'));
}

