<?php
// $Id$

/**
 * Alma authentication form.
 *
 * Only used when a logged in user requires access to data from Alma and
 * his Alma credentials are not cached.
 *
 * @param array $form_state
 *    Drupal's form state.
 * @param stdClass $account
 *    User account object.
 * @param mixed $explanation
 *    Explanation for the login form. NULL for the default message.
 * @param mixed $redirect
 *    Redirect destination – boolean TRUE for default destination
 *    (user's profile page).
 */
function alma_user_forms_alma_authenticate($form_state, $account, $explanation=NULL, $redirect=TRUE) {
  $form = array();

  $form['redirect'] = array(
    '#type' => 'value',
    '#value' => $redirect,
  );

  // Strict NULL check to allow an empty explanation string.
  if (is_null($explanation)) {
    $explanation = t('To access your personal information, you need to authenticate.');
  }

  $form['explanation'] = array(
    '#value' => $explanation,
  );

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['borr_card'] = array(
    '#type' => 'password',
    '#title' => t('Borrower card'),
    '#required' => TRUE,
  );

  $form['pin_code'] = array(
    '#type' => 'password',
    '#title' => t('Code'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Log in'),
  );

  return $form;
}

/**
 * Validation for authentication form.
 */
function alma_user_forms_alma_authenticate_validate($form, &$form_state) {
  // Lookup user data
  $data = alma_client_get_patron_info($form_state['values']['borr_card'], $form_state['values']['pin_code']);

  // If we got no data or account IDs do not match, it usually means that the
  // user supplied the wrong credentials.
  if (!$data || $data['patron_id'] != $form_state['values']['account']->alma_id) {
    form_set_error('borr_card', t('Invalid borrower card or pin code'));
  }
}

/**
 * Submit handler for authentication form.
 */
function alma_user_forms_alma_authenticate_submit($form, &$form_state) {
  // Validation succeeded, so cache the correct credentials.
    alma_user_cache_credentials($form_state['values']['account'], $form_state['values']['borr_card'], $form_state['values']['pin_code']);

  // If we do not have a destination already, use the redirect value.
  if (!isset($_REQUEST['destination']) && $form_state['values']['redirect']) {
    // If redirect is boolean TRUE, use the default redirect.
    if (is_bool($form_state['values']['redirect'])) {
      $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
    }
    else {
      // Otherwise, assume that the redirect value is a sane destination.
      $form_state['redirect'] = $form_state['values']['redirect'];
    }
  }
}

/**
 * Change PIN code form.
 */
function alma_user_forms_change_pin($form_state, $account) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['current_pin'] = array(
    '#type' => 'password',
    '#title' => t('Current PIN code'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#size' => 4,
  );

  $form['new_pin'] = array(
    '#type' => 'password',
    '#title' => t('New PIN code'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#size' => 4,
  );

  $form['new_pin_confirm'] = array(
    '#type' => 'password',
    '#title' => t('Repeat new PIN code'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#size' => 4,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save new PIN code'),
  );

  return $form;
}

/**
 * Validation for PIN code changing form.
 */
function alma_user_forms_change_pin_validate($form, &$form_state) {
  $creds = alma_user_get_credentials($form_state['values']['account']);
  if ($form_state['values']['current_pin'] != $creds['pin_code']) {
    form_set_error('current_pin', t('Please provide your current PIN code.'));
  }

  if ($form_state['values']['current_pin'] == $form_state['values']['new_pin']) {
    form_set_error('new_pin', t('The new PIN code must be different than the current PIN code.'));
  }

  if ($form_state['values']['new_pin_confirm'] != $form_state['values']['new_pin']) {
    form_set_error('new_pin_confirm', t('Please input your new PIN code twice for confirmation.'));
  }

  if (!ctype_digit($form_state['values']['new_pin']) || strlen($form_state['values']['new_pin']) != 4) {
    form_set_error('new_pin', t('The new PIN code must be four numerical digits.'));
  }
}

/**
 * Submit handler for the PIN code changing form.
 */
function alma_user_forms_change_pin_submit($form, &$form_state) {
  $creds = alma_user_get_credentials($form_state['values']['account']);
  alma_client_change_pin($creds['borr_card'], $creds['pin_code'], $form_state['values']['new_pin']);

  // Cache the new pin-code
  alma_user_cache_credentials($form_state['values']['account'], $creds['borr_card'], $form_state['values']['new_pin']);

  // If we do not have a destination already, go to the profile.
  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
  }
}

/**
 * User library settings form.
 */
function alma_user_forms_library_settings($form_state, $account) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['preferred_branch'] = array(
    '#type' => 'select',
    '#title' => t('Preferred library'),
    '#options' => alma_client_get_reservation_branches(),
    '#default_value' => $account->preferred_branch,
  );

  $period_options = array();
  foreach (range(1, 25, 2) as $num) {
    $period_options[$num] = format_plural($num, '1 month', '@count months');
  }

  $form['interest_period'] = array(
    '#type' => 'select',
    '#title' => t('Period of interest'),
    '#description' => t('How long are you willing to wait for a reservation?'),
    '#options' => $period_options,
    '#default_value' => 6,
  );

  $form['suspend_start'] = array(
    '#type' => 'date_popup',
    '#title' => t('Suspend reservations from'),
    '#date_format' => DATE_FORMAT_DATE,
  );

  $form['suspend_end'] = array(
    '#type' => 'date_popup',
    '#title' => t('To'),
    '#date_format' => DATE_FORMAT_DATE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Validation for user library settings.
 */
function alma_user_forms_library_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['suspend_start']) && !empty($form_state['values']['suspend_end'])) {
    $start = strtotime($form_state['values']['suspend_start']);
    $end = strtotime($form_state['values']['suspend_end']);

    if ($start < $_SERVER['REQUEST_TIME']) {
      form_set_error('suspend_start', t('Start date must later than today'));
    }

    if ($start >= $end) {
      form_set_error('suspend_start', t('Start date must be before end date'));
    }
    else {
      $form_state['values']['suspend'] = array('start' => $start, 'end' => $end);
    }
  }
}


/**
 * Submit handler for user library settings.
 */
function alma_user_forms_library_settings_submit($form, &$form_state) {
  db_query("UPDATE {alma_user} SET preferred_branch = '%s' WHERE uid = %d LIMIT 1;", $form_state['values']['preferred_branch'], $form_state['values']['account']->uid);
}

/**
 * Reservation details form.
 */
function alma_user_forms_reservation_details($form_state, $account, $records) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['reservation_data'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  $form['reservations'] = array(
    '#tree' => TRUE,
  );

  foreach ($records as $reservation) {
    $form['reservation_data']['#value'][$reservation['id']] = $reservation;
    if ($reservation['status'] == 'active') {
      $form['reservations'][$reservation['id']] = array(
        '#type' => 'checkbox',
      );
    }
    else {
      // If not selectable, output an empty column.
      $form['reservations'][$reservation['id']] = array(
        '#value' => '&nbsp;',
      );
    }
  }

  $form['new_expiry'] = array(
    '#type' => 'date_popup',
    '#title' => t('Not interested after'),
    '#date_format' => DATE_FORMAT_DATE,
    '#date_year_range' => '-0:+2',
  );


  $form['new_pickup_branch'] = array(
    '#type' => 'select',
    '#title' => t('New pickup branch'),
    '#options' => alma_client_get_reservation_branches(),
    '#default_value' => $account->preferred_branch,
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel selected'),
    '#attributes' => array('class' => 'cancel-button'),
  );

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update selected'),
    '#attributes' => array('class' => 'update-button'),
  );

  return $form;
}

/**
 * Reservation details form validation.
 */
function alma_user_forms_reservation_details_validate($form, &$form_state) {
  if (!empty($form_state['values']['new_expiry'])) {
    list($year, $month, $day) = explode('-', trim($form_state['values']['new_expiry']));
    if (!checkdate($month, $day, $year)) {
      form_set_error('new_expiry', t('Invalid date'));
    }

    $timestamp = strtotime($form_state['values']['new_expiry']);

    if ($timestamp <= $SERVER['REQUEST_TIME']) {
      form_set_error('new_expiry', t('Date must be later than today.'));
    }
    else {
      form_set_value($form['new_expiry'], format_date($timestamp, 'custom', 'Y-m-d'), $form_state);
    }
  }
}

/**
 * Reservation details form submit handler.
 */
function alma_user_forms_reservation_details_submit($form, &$form_state) {
  $selected = array();
  $reservations = $form_state['values']['reservation_data'];

  foreach ($form_state['values']['reservations'] as $id => $val) {
    if ($val == 1) {
      $selected[] = $id;
    }
  }

  switch ($form_state['clicked_button']['#attributes']['class']) {
    case 'update-button':
      $changes = array();
      if (!empty($form_state['values']['new_expiry'])) {
        $changes['valid_to'] = $form_state['values']['new_expiry'];
      }

      if (!empty($form_state['values']['new_pickup_branch'])) {
        $changes['pickup_branch'] = $form_state['values']['new_pickup_branch'];
      }

      if (!empty($selected) && !empty($changes)) {
        $creds = alma_user_get_credentials($form_state['values']['account']);
        foreach ($selected as $id) {
          alma_client_change_reservation($creds['borr_card'], $creds['pin_code'], $reservations[$id], $changes);
        }
      }
      break;
    case 'cancel-button':
      $creds = alma_user_get_credentials($form_state['values']['account']);
      foreach ($selected as $id) {
        alma_client_remove_reservation($creds['borr_card'], $creds['pin_code'], $reservations[$id]);
      }
      break;
  }
}

/**
 * Loan details form.
 */
function alma_user_forms_loan_details($form_state, $account, $records) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['loan_data'] = array(
    '#type' => 'value',
    '#grouped' => _alma_user_forms_group_loans($records),
    '#value' => $records,
  );

  $form['loans'] = array(
    '#tree' => TRUE,
  );

  foreach ($records as $loan) {
    if ($loan['is_renewable']) {
      $form['loans'][$loan['id']] = array(
        '#type' => 'checkbox',
      );
    }
    else {
      $form['loans'][$loan['id']] = array(
        '#value' => t('Not renewable'),
      );
    }
  }

  $form['renew'] = array(
    '#type' => 'submit',
    '#value' => t('Renew selected'),
  );

  return $form;
}

/**
 * Loan details submit handler.
 */
function alma_user_forms_loan_details_submit($form, $form_state) {
  $selected = array();

  foreach ($form_state['values']['loans'] as $key => $value) {
    if ($value) {
      $selected[] = $key;
    }
  }

  $creds = alma_user_get_credentials($form_state['values']['account']);
  alma_client_renew_loan($creds['borr_card'], $creds['pin_code'], $selected);
}

/**
 * Helper function to group loans by their due dates.
 */
function _alma_user_forms_group_loans($records) {
  $groups = array();

  foreach ($records as $loan) {
    if ($loan['due_date'] <= date('Y-m-d')) {
      $groups['overdue'][] = $loan;
    }
    else {
      $groups[$loan['due_date']][] = $loan;
    }
  }
  return $groups;
}

/**
 * Debts details form.
 */
function alma_user_forms_debts_details(&$form_state, $account, $debt_data) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['debt_data'] = array(
    '#type' => 'value',
    '#value' => $debt_data,
  );

  $form['debts'] = array(
    '#tree' => TRUE,
  );

  foreach ($debt_data['debts'] as $debt) {
    $form['debts'][$debt['id']] = array(
      '#type' => 'checkbox',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Pay now'),
  );

  return $form;
}

/**
 * Validation for debts details form.
 */
function alma_user_forms_debts_details_validate($form, &$form_state) {
  $selected_debts = array_keys(array_filter($form_state['values']['debts']));

  if (empty($selected_debts)) {
    form_set_error('debts', t('Please select one or more debts to pay.'));
  }
  $form_state['values']['selected_debts'] = $selected_debts;

  $total = 0.0;

  foreach ($selected_debts as $debt_id) {
    $total += (float) $form_state['values']['debt_data']['debts'][$debt_id]['amount'];
  }

  if ($total <= 0.0) {
    form_set_error('debts', t('Payment must be larger than zero.'));
  }
  $form_state['values']['total_amount'] = $total;
}

