<?php // $Id$

/**
 * @file ding_library.admin.inc
 * Admin forms, page callbacks and related functions.
 */

/**
 * The main settings form.
 */
function ding_library_admin_settings_main_form($form_state) {
  $form = array();

  $form['placeholder'] = array(
    '#value' => 'This page intentionally left blank.',
  );

  return system_settings_form($form);
}

/**
 * The library ordering page.
 *
 * Provides a tabledrag sorting view of all the libraries, so they can 
 * be ordered as per the users preference.
 */
function ding_library_admin_ordering_page() {
  return 'All your em-space are belong to us.';
}

