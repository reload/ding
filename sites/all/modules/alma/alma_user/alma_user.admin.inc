<?php
// $Id$

/**
 * Alma user settings form.
 */
function alma_user_admin_settings_form($form_state) {
  $form = array(
    // We need to set the submit handler explicitly, because the 
    // #submit array is appended to by system_settings_form.
    '#submit' => array('alma_user_admin_settings_form_submit'),
  );

  $form['alma_user_secure_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Secure roles'),
    '#description' => t('Select the roles that are high security, requiring username/password login instead the simple Alma patron login. Note that the superuser account (uid=1) is always considered to have a secure role.'),
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('alma_user_secure_roles', array()),
  );

  $form['alma_user_credential_cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Credential cache duration'),
    '#description' => t('Determines how long the user’s Alma credentials will be cached for. For the duration of the cache, the user will be able to access his personal information and interact with the library system without having to provide his credentials again.'),
    '#options' => array(
      300 => t('5 minutes'),
      900 => t('15 minutes'),
      1800 => t('30 minutes'),
      3600 => t('1 hour'),
      10800 => t('3 hours'),
      43200 => t('12 hours'),
      86400 => t('1 day'),
      259200 => t('3 days'),
      604800 => t('7 days'),
    ),
    '#default_value' => variable_get('alma_user_credential_cache_duration', 1800),
  );

  return system_settings_form($form);
}

/**
 * Submit handler for user settings form.
 */
function alma_user_admin_settings_form_submit($form, &$form_state) {
  // Removed unchecked (value == 0) values from the secure roles array.
  // This makes it possible to use a simple intersect to figure out if 
  // the user has a secure role.
  foreach (array_keys($form_state['values']['alma_user_secure_roles'], 0) as $key) {
    unset($form_state['values']['alma_user_secure_roles'][$key]);
  }
}

