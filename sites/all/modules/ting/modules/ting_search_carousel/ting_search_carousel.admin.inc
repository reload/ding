<?php
// $Id$

/**
 * Rules editing page callback.
 *
 * Simple wrapper for the form, only required to add the JavaScript.
 */
function ting_search_carousel_admin_page() {
  // Use the admin module's theme if available.
  if (function_exists('_admin_init_theme')) {
    _admin_init_theme();
  }
  
  $path = drupal_get_path('module', 'ting_search_carousel');
  drupal_add_js($path . '/js/ting_search_carousel.admin.js');
  drupal_add_css($path . '/css/ting_search_carousel.css');
  
  return drupal_get_form('ting_search_carousel_admin_form');
}

/**
 * Ting search carousel admin form.
 */
function ting_search_carousel_admin_form($form_state) {
  $form = array();

  $form['ting_search_carousel_searches'] = array(
    '#type' => 'fieldset',
    '#title' => t('Searches'),
    '#attributes' => array('id' => 'ting-search-carousels'),
    '#tree' => TRUE,
  );
  	
  $searches = variable_get('ting_search_carousel_searches', array());
  if ($searches) {
    foreach ($searches as $delta => $search) {
      $form['ting_search_carousel_searches'][$delta] = array(
        '#type' => 'ting_search_carousel_search',
        '#value' => $search
      );
    }
  }
  else {
    $form['ting_search_carousel_searches'][0] = array(
      '#type' => 'ting_search_carousel_search'
    );
  }  

  $form['new_search_carousel_button'] = array(
    '#type' => 'button',
    '#value' => 'Add another',
    '#ahah' => array(
      'path' => 'admin/settings/ting_search_carousel/form/ahah',
      'wrapper' => 'ting-search-carousels',
      'method' => 'append',
      'effect' => 'slide'
    ),
  );

  return system_settings_form($form);    
}

/*
  This function is largely based on the poll module, its been simplified for reuse.
  $fields is the specific form elements you want to attach via ahah,
  $name is the form fields array key... e.g. the name for $form['title'] is "title"
*/ 
function ting_search_carousel_form_ahah($form_state) {
  // Use the admin module's theme if available.
  if (function_exists('_admin_init_theme')) {
    _admin_init_theme();
  }
  
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);

  // Delta is assigned to the count of current elements, so it'll be the 
  // next in the zero-based list.
  $delta = count(element_children($form['ting_search_carousel_searches']));

  $form['ting_search_carousel_searches'][$delta] = array(
    '#type' => 'ting_search_carousel',
    '#delta' => $delta,
  );
  form_set_cache($form_build_id, $form, $form_state);
  
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);
  
  // Render and return the new field.
  drupal_json(array('data' => drupal_render($form['ting_search_carousel_searches'][$delta]), 'status' => TRUE));
  exit();
}