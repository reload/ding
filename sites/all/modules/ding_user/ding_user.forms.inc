<?php
// $Id$

/**
 * Change admin password form.
 */
function ding_user_forms_change_pass($form_state, $account) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

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
function ding_user_forms_change_pass_validate($form, &$form_state) {
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
function ding_user_forms_change_pass_submit($form, &$form_state) {
  $account = $form_state['values']['account'];

  // If there is not a secure pass set on the user object, we assume that
  // it is because the user has no record in ding_user. Create one.
  if (!isset($account->secure_pass) || !$account->secure_pass || empty($account->secure_pass)) {
    db_query("INSERT INTO {ding_user} (uid, pass) VALUES (%d, '%s');", $account->uid, user_hash_password($form_state['values']['new_secure_pass']));
  }
  else {
    db_query("UPDATE {ding_user} SET pass = '%s' WHERE uid = %d LIMIT 1;", user_hash_password($form_state['values']['new_secure_pass']), $account->uid);
  }

  drupal_set_message(t('Your new password has been saved.'));
}

