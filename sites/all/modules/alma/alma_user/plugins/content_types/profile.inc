<?php
// $Id$
/**
 * @file relevant.inc
 * Provides several library portal page related panels content types.
 */

/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function alma_user_profile_ctools_content_types() {
  return array(
    'title' => t('Campaign'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function alma_user_profile_content_type_content_types() {
  $types = array(
    'alma_user_personal_info' => array(
      'title' => t('Personal information'),
      'description' => t('Display of the patron’s personal information, with options to edit them.'),
    ),
    'alma_user_library_settings' => array(
      'title' => t('Library settings'),
      'description' => t('The patron’s library settings. Allows the patron to choose a preferred library and suspend his reservations for a period.'),
    ),
    'alma_user_reservations_list' => array(
      'title' => t('List of reservations'),
      'description' => t('A simple list of the patron’s reserved books.'),
    ),
    'alma_user_loans_list' => array(
      'title' => t('List of loans'),
      'description' => t('A simple list of the patron’s reserved loans.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library patron profile'),
      'required context' => new ctools_context_required(t('Patron'), 'user'),
    );
  }
  
  return $types;
}

/**
 * Return the pane contents.
 */
function alma_user_profile_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $alma_creds = alma_user_get_credentials($account); 
  $block = new stdClass();

  if ($alma_creds) {
    switch ($subtype) {
      case 'alma_user_personal_info':
        $data = alma_client_get_patron_info($alma_creds['borr_card'], $alma_creds['pin_code']);
        $block->title = t('Your profile');
        $block->content = theme('alma_user_personal_info', $account, $data);
        break;
      case 'alma_user_library_settings':
        module_load_include('forms.inc', 'alma_user');
        $block->title = t('Your library');
        $block->content = drupal_get_form('alma_user_forms_library_settings', $account);
        break;
      case 'alma_user_reservations_list':
        $block->title = t('Your reservations');
        $list = alma_client_get_reservations($alma_creds['borr_card'], $alma_creds['pin_code']);
        $block->content = theme('alma_user_record_list', $list);
        break;
      case 'alma_user_loans_list':
        $block->title = t('Your loans');
        $list = alma_client_get_loans($alma_creds['borr_card'], $alma_creds['pin_code']);
        $block->content = theme('alma_user_record_list', $list);
        break;
    }
  }
  elseif ($subtype == 'alma_user_personal_info') {
    $block->content = t('Error fetching your library patron profile.');
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function alma_user_profile_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'alma_user_personal_info':
      return t('“@s”: Personal information', array('@s' => $context->identifier));
    case 'alma_user_library_settings':
      return t('“@s”: Library settings', array('@s' => $context->identifier));
    case 'alma_user_reservations_list':
      return t('“@s”: Reservations list', array('@s' => $context->identifier));
    case 'alma_user_loans_list':
      return t('“@s”: Loans list', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook 
 * for this content type to work.
 */
function alma_user_profile_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

