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

/*  if ($GLOBALS['user']->uid == $account->uid && !empty($account->secure_pass)) {
    $form['secure_pass'] = array(
      '#type' => 'password',
      '#title' => t('Current password'),
      '#required' => TRUE,
    );
  }*/

  $form['new_secure_pass'] = array(
    '#type' => 'password_confirm',
    '#title' => t('New password'),
    '#required' => TRUE,
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
  dd($data);
}

/**
 * Submit handler for authentication form.
 */
function alma_user_forms_alma_authenticate_submit($form, &$form_state) {
  // Validation succeeded, so cache the correct credentials.
  cache_set($form_state['values']['account']->alma_id, $form_state['values']['borr_card'] . '|' . $form_state['values']['pin_code'], 'cache_alma_credentials', $_SERVER['REQUEST_TIME'] + 1800);

  // If we do not have a destination already, go to the profile.
  if (isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
  }
}

