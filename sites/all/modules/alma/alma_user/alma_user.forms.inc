<?php
// $Id$

/**
 * Alma admin password form.
 */
function alma_user_forms_admin_pass($form_state, $account) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  if ($GLOBALS['user']->uid == $account->uid && !empty($account->secure_pass)) {
    $form['secure_pass'] = array(
      '#type' => 'password',
      '#title' => t('Current password'),
      '#required' => TRUE,
    );
  }

  $form['new_secure_pass'] = array(
    '#type' => 'password_confirm',
    '#title' => t('New password'),
    '#required' => TRUE,
    '#description' => t('The new password must have at least one letter, one number, and must be a least 8 characters long.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save new password'),
  );

  return $form;
}

/**
 * Validation of the admin password form.
 */
function alma_user_forms_admin_pass_validate($form, &$form_state) {
  if (isset($form['secure_pass'])) {
    if (!alma_user_check_password($form_state['values']['secure_pass'], $form_state['values']['account'])) {
      form_set_error('secure_pass', t('Please enter your current password.'));
    }
  }

  if (drupal_strlen($form_state['values']['new_secure_pass']) < 8) {
    form_set_error('new_secure_pass', t('The new password must be a least 8 characters long.'));
  }

  if (!preg_match('/[A-Za-zæøåÆØÅ]/', $form_state['values']['new_secure_pass'])) {
    form_set_error('new_secure_pass', t('The new password must have at least one letter.'));
  }

  if (!preg_match('/\d/', $form_state['values']['new_secure_pass'])) {
    form_set_error('new_secure_pass', t('The new password must have at least one number.'));
  }
}

/**
 * Submit handler for the admin password form.
 */
function alma_user_forms_admin_pass_submit($form, &$form_state) {
  $account = $form_state['values']['account'];

  if (empty($account->secure_pass)) {
    db_query("INSERT INTO {alma_user_pass} (uid, pass) VALUES (%d, '%s');", $account->uid, user_hash_password($form_state['values']['new_secure_pass']));
  }
  else {
    db_query("UPDATE {alma_user_pass} SET pass = '%s' WHERE uid = %d LIMIT 1;", user_hash_password($form_state['values']['new_secure_pass']), $account->uid);
  }
}

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
 * Change contact information form.
 */
function alma_user_forms_edit_contact_info($form_state, $account) {
  $form = array();

  return $form;
}

