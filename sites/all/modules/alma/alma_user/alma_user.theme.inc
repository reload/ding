<?php
// $Id$

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

/**
 * Theme hook for our reservation element.
 */
function theme_alma_loan($element) {
  return $element['#children'];
}

