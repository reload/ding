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

