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

  // Get the credentials to make sure this session is properly
  // authenticated before displaying the cart contents.
  $creds = alma_user_get_credentials($account);

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
    $form['cart_data']['#value'][$row['alma_id']] = $row;
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
    // Default expiry is one year from now.
    '#default_value' => intval(date('Y', $_SERVER['REQUEST_TIME'])) + 1 . date('-m-d', $_SERVER['REQUEST_TIME']),
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

/**
 * Validation for cart details form.
 */
function alma_cart_forms_cart_details_validate($form, &$form_state) {
  // Strip off the time part of the return value.
  $form_state['values']['expiry'] = substr($form_state['values']['expiry'], 0, 10);

  if (date('Y-m-d', $_SERVER['REQUEST_TIME']) >= $form_state['values']['expiry']) {
    form_set_error('expiry', t('Date must be in the future.'));
  }
}

/**
 * Submit handler for cart details form.
 */
function alma_cart_forms_cart_details_submit($form, &$form_state) {
  $selected_items = array();

  foreach ($form_state['values']['cart'] as $local_id => $value) {
    if ($value == 1) {
      $selected_items[] = $local_id;
    }
  }

  if (!empty($selected_items)) {
    // For some reason ['#parents'][0] seems to be the only way to
    // reliably access the button's element name.
    switch ($form_state['clicked_button']['#parents'][0]) {
      case 'reserve':
        $creds = alma_user_get_credentials($form_state['values']['account']);

        foreach ($selected_items as $item) {
          $reservation = alma_client_add_reservation($creds['borr_card'], $creds['pin_code'], array(
            'id' => $item,
            'valid_to' => $form_state['values']['expiry'],
            'pickup_branch' => $form_state['values']['pickup_branch'],
          ));

          // If reservation was successful, remove the item from the cart.
          if ($reservation) {
            db_query("DELETE FROM {alma_cart} WHERE uid = %d AND alma_id = '%s');", $form_state['values']['account']->uid, $item);
          }
          else {
            drupal_set_message(t('Reservation of “@title” failed. It may be on your reservation list already.', array('@title' => $form['cart_data']['#value'][$item]['title'])), 'error');
          }
        }
        break;
      case 'remove':
        db_query("DELETE FROM {alma_cart} WHERE uid = %d AND alma_id IN (" . join(',', $selected_items) . ");", $form_state['values']['account']->uid);
        break;
    }
  }
}

