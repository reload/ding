<?php
// $Id$

/**
 * AHAH callback to get library item availability.
 *
 * @param string $input
 *    Input string, one or more Alma item IDs, comma-separated.
 */
function alma_item_availability_page($input) {
  $data = alma_client_get_availability($input);

  // We are returning JavaScript, so tell the browser.
  drupal_set_header('Content-Type: application/json; charset=utf-8');
  echo json_encode($data);
  return;
}

/**
 * AHAH callback to get library item details.
 *
 * @param string $input
 *    Input string, one or more Alma item IDs, comma-separated.
 */
function alma_item_details_page($input) {
  // Split input on commas, filter out bogus values.
  $numbers = array_filter(explode(',', $input), is_numeric);
  $items = array();

  foreach ($numbers as $alma_id) {
    $items[$alma_id] = alma_client_catalogue_record_detail($alma_id);
  }

  // We are returning JavaScript, so tell the browser.
  drupal_set_header('Content-Type: application/json; charset=utf-8');
  echo json_encode($items);
  return;
}

