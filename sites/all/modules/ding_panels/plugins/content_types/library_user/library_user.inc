<?php // $Id$

/**
 * @file library_users.inc
 * Provides several library portal page related panels content types.
 */

/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function ding_panels_library_user_ctools_content_types() {
  return array(
    'title' => t('Library user'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function ding_panels_library_user_content_type_content_types() {
  $types = array(
    'library_user_staff_title' => array(
      'title' => t('Library staff title'),
      'description' => t('The title bar for a library staff member.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library user'),
      'required context' => new ctools_context_required(t('User'), 'user'),
    );
  }
  return $types;
}

/**
 * Return the pane contents.
 */
function ding_panels_library_user_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $user = isset($context->data) ? drupal_clone($context->data) : NULL;
  switch($subtype) {
    case 'library_user_staff_title':
      $block->content = theme('ding_panels_library_user_staff_title', $user);
      break;
  }
  return $block;
}

/**
 * Return the pane titles.
 */
function ding_panels_library_user_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'library_user_staff_title':
      return t('"@s" staff profile', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook 
 * for this content type to work.
 */
function ding_panels_library_user_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

