<?php // $Id$

/**
 * @file library_portal.inc
 * Provides several library portal page related panels content types.
 */


/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function ding_panels_library_portal_ctools_content_types() {
  return array(
    'title' => t('Library page'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function ding_panels_library_portal_content_type_content_types() {
  $types = array(
    'library_portal_navigation' => array(
      'title' => t('Library portal navigation.'),
      'description' => t('Shows the CMS menu - if available, it will show the portal library specific menu.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library page'),
      'required context' => new ctools_context_optional(t('Node'), 'node'),
    );
  }
  return $types;
}

/**
 * Return the pane contents.
 */
function ding_panels_library_portal_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $library = isset($context->data) ? drupal_clone($context->data) : NULL;
  switch($subtype) {
    case 'library_portal_navigation':
      $block->content = _ding_panels_library_portal_navigation_render($library, $conf['default_menu']);
      break;
  }
  return $block;
}

/**
 * Return the pane titles.
 */
function ding_panels_library_portal_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
  case 'library_portal_navigation':
    return t('"@s":  Library portal navigation', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 */
function ding_panels_library_portal_content_type_edit_form(&$form, &$form_state) {
  switch($form_state['subtype_name']) {
    case 'library_portal_navigation':
      $form['default_menu'] = array(
        '#type' => 'select',
        '#title' => t('Default menu'),
        '#description' => t('If a menu related to the library node can not be found, show this menu instead.'),
        '#options' => array_merge(array(t('- None -')), menu_get_menus()),
        '#default_value' => $form_state['conf']['default_menu'],
      );

      // Add a 'none' option to the menu options.
      array_unshift($form['default_menu']['#default_value'], t('- None -'));
      break;
  }

  return $form;
}

/**
 * Submit handler for the content type editing form.
 */
function ding_panels_library_portal_content_type_edit_form_submit(&$form, &$form_state) {
  switch($form_state['subtype_name']) {
    case 'library_portal_navigation':
      $form_state['conf']['default_menu'] = $form_state['values']['default_menu'];
      break;
  }
}

/**
 * Render the menu for a portal page.
 *
 * @param object $node
 *    The library node.
 * @param string $default_menu
 *    Menu ID of the menu to use if a suitable library menu was not found.
 * @return string
 *    Rendered menu.
 */
function _ding_panels_library_portal_navigation_render($library = NULL, $default_menu = NULL) {
  // If a library node is specified, try to load its menu tree.
  if ($library instanceof stdClass) {
    $tree = menu_tree('ding_library_' . $library->nid);
  }

  if (empty($tree) && !empty($default_menu)) {
    // Load the default menu.
    $tree = menu_tree($default_menu);
  }

  if (!empty($tree)) {
    return $tree;
  }
}


