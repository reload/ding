<?php
// $Id$

/**
 * User cart details form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param stdClass $account
 *    The user object for the cart we're looking at.
 * @return array
 */
function alma_cart_forms_cart_details(&$form_state, $account) {
  $form = array();
  
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['cart_data'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  // Fetch the cart data from the database.
  $query = db_query("SELECT * FROM {alma_cart} WHERE uid = %d;", $account->uid);
  while ($row = db_fetch_array($query)) {
    $form['cart_data']['#value'][] = $row;
  }

  $form['cart'] = array(
    // Set as tree to get all of our checkboxes as an subarray of values
    // in the form_state['values'] array.
    '#tree' => TRUE,
  );

  foreach ($form['cart_data']['#value'] as $item) {
    $form['cart'][$item['alma_id']] = array(
      '#type' => 'checkbox',
    );
  }

  $form['expiry'] = array(
    '#type' => 'date_popup',
    '#title' => t('Not interested after'),
    '#date_format' => DATE_FORMAT_DATE,
    '#date_year_range' => '-0:+2',
  );

  $form['pickup_branch'] = array(
    '#type' => 'select',
    '#title' => t('Pickup branch'),
    '#options' => alma_client_get_reservation_branches(),
    '#default_value' => $account->preferred_branch,
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove selected'),
    '#attributes' => array('class' => 'remove-button'),
  );

  $form['reserve'] = array(
    '#type' => 'submit',
    '#value' => t('Reserve selected'),
    '#attributes' => array('class' => 'reserve-button'),
  );

  return $form;
}

