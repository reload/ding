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

