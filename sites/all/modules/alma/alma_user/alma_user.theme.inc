<?php
// $Id$

/**
 * Preprocessor for the user status template.
 */
function template_preprocess_alma_user_status_block(&$variables) {
  $variables['wrapper_classes'] = '';
  // Use alma_name if available.
  if (!empty($variables['user']->alma_name)) {
    $variables['display_name'] = $variables['user']->alma_name;
  }
  else {
    $variables['display_name'] = $variables['user']->name;
  }

  if (empty($variables['user_status'])) {
    $variables['user_status_unavailable'] = TRUE;
    if (!empty($variables['user']->alma_id)) {
      $variables['status_unavailable_message'] = t('Please !log_in to view your loaner status', array('!log_in' => l(t('log in'), 'user/' . $variables['user']->uid . '/authenticate')));
    }
    else {
      // If user doesn't have an Alma ID, don't show a message, since we
      // can't just log him in to get the status. This should usually
      // only be the case for admin users.
      $variables['status_unavailable_message'] = '';
    }
  }
  elseif ($variables['user_status']['loan_overdue_count'] > 0) {
    $variables['wrapper_classes'] .= 'loans-overdue';
  }

  // Since the cart is not implemented yet, we'll just set its count to 0
  $variables['cart_count'] = 0;
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

/**
 * Theme hook for our reservation element.
 */
function theme_alma_reservation($element) {
  return $element['#children'];
}

