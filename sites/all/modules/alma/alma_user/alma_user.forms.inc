<?php
// $Id$

/**
 * Alma authentication form.
 *
 * Only used when a logged in user requires access to data from Alma and
 * his Alma credentials are not cached.
 */
function alma_user_forms_alma_authenticate($form_state, $account) {
  $form = array();

  $form['explanation'] = array(
    '#value' => t('To access your personal information, you need to authenticate.'),
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

  // If we do not have a destination already, go to the profile.
  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
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
  // TODO: Save new PIN code to Alma.
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

  $form['reservations'] = array(
    '#tree' => TRUE,
    '#prefix' => '<ul class="reservations"><ul class="labels"><li class="select">' . t('Select') . '</li><li class="title">' . t('Reservations') . '</li><li class="expires">' . t('Expires') . '</li><li class="pickup-branch">' . t('Pickup branch') . '</li><li class="queue">' . t('Queue position') . '</li></ul>',
    '#suffix' => '</ul>',
  );

  foreach ($records as $rec) {
    $form['reservations'][$rec['id']] = array(
      '#type' => 'alma_reservation',
      '#reservation' => $rec,
    );
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

  foreach ($form_state['values']['reservations'] as $id => $res) {
    if ($res['selected']) {
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
          alma_client_change_reservation($creds['borr_card'], $creds['pin_code'], $form['reservations'][$id]['#reservation'], $changes);
        }
      }
      break;
    case 'cancel-button':
      $creds = alma_user_get_credentials($form_state['values']['account']);
      foreach ($selected as $id) {
        alma_client_remove_reservation($creds['borr_card'], $creds['pin_code'], $form['reservations'][$id]['#reservation']);
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

  $form['loans'] = array(
    '#tree' => TRUE,
  );

  foreach (_alma_user_forms_group_loans($records) as $date => $group) {
    if ($date == 'overdue') {
      $form['loans'][$date] = array(
        '#value' => t('Overdue loans'),
        '#prefix' => '<h4 class="loan-group overdue">',
        '#suffix' => '</h4>',
      );
    }
    else {
      $form['loans'][$date] = array(
        '#value' => t('Due in @count days, @date', array('@date' => $date, '@count' => ceil((strtotime($date) - $_SERVER['REQUEST_TIME']) / 86400))),
        '#prefix' => '<h4 class="loan-group">',
        '#suffix' => '</h4>',
      );
    }

    foreach ($group as $loan) {
      $form['loans'][$loan['id']] = array(
        '#type' => 'alma_loan',
        '#loan' => $loan,
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

  foreach ($form_state['values']['loans'] as $key => $loan) {
    if ($loan['selected']) {
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

