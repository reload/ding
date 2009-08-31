<?php
// $Id: google404.admin.inc,v 1.1 2009/08/31 10:52:06 mikl Exp $

/**
 * Admin settings form.
 */
function google404_admin_settings_form($form_state) {
  $form = array();

  $form['message_filter']['google404_message'] = array(
    '#type' => 'textarea',
    '#title' => t('404 message'),
    '#description' => t('The message that should be displayed on the 404 page.'),
    '#default_value' => variable_get('google404_message', t('Sorry, the page you were looking for could not be found.')),
    '#rows' => 10,
    '#required' => TRUE,
  );

  $form['message_filter']['format'] = filter_form(variable_get('google404_message_format', FILTER_FORMAT_DEFAULT));
  

  return system_settings_form($form);
}

/**
 * Validation for admin settings form.
 */
function google404_admin_settings_form_validate($form, &$form_state) {
  // Since wysiwyg.module requires us to name our format picker 
  // "format", we'll have to rename the returned value to have it stored 
  // correctly by system_settings_form().
  $form_state['values']['google404_message_format'] = $form_state['values']['format'];
}

