<?php
// $Id$

/**
 * Page callback to handle adding stuff to the cart.
 *
 * Handles both AJAX and regular requests, presenting the user with the 
 * opportunity to log in if needed. 
 *
 * @param TingClientObject $object
 */
function alma_cart_page_add_to_cart($object) {
  // First, let's check if this is in the cart already :)
  $exists = (boolean) db_result(db_query("SELECT created FROM {alma_cart} WHERE uid = %d AND alma_id = '%s';", $GLOBALS['user']->uid, $object->data->localId));
  if ($exists) {
    drupal_set_message('%book is already in your cart.', array('%book' => $object->data->title[0]));
    return drupal_goto('user/' . $GLOBALS['user']->uid . '/cart');
  }

  $creds = alma_user_get_credentials($GLOBALS['user'], FALSE);
  if ($creds) {
    // Present a confirmation form.
    return drupal_get_form('alma_cart_confirm_form', $object, 'add');
  }
  else {
    // Require user login.
    module_load_include('forms.inc', 'alma_user');
    return drupal_get_form('alma_user_forms_alma_authenticate', $GLOBALS['user'],t('To add books to your cart, you need to log in.'), FALSE); 
  } 

  return drupal_not_found(); 
}

/**
 * Alma cart confirmation form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param TingClientObject $object
 *    The object we're acting on.
 * @param string $action
 *    The type of action we're taking.
 */
function alma_cart_confirm_form(&$form_state, $object, $action) {
  // Question keyed by action.
  $questions = array(
    'add' => t('Add %book to your cart?', array('%book' => $object->data->title[0])),
    'reserve' => t('Reserve %book immediately?', array('%book' => $object->data->title[0])),
  );

  // Destination URL keyed by action.
  $urls = array(
    'add' => 'user/' . $GLOBALS['user']->uid . '/cart',
    'reserve' => 'user/' . $GLOBALS['user']->uid . '/status',
  );


  // Send some form values along for the submit step.
  $form = array();

  $form['object'] = array(
    '#type' => 'value',
    '#value' => $object,
  );

  $form['action'] = array(
    '#type' => 'value',
    '#value' => $action,
  );

  $form['dest_url'] = array(
    '#type' => 'value',
    '#value' => $urls[$action],
  );

  return confirm_form($form, $questions[$action], $urls[$action], '');
}

/**
 * Submit handler for Alma cart confirmation form.
 */
function alma_cart_confirm_form_submit($form, &$form_state) {
  dpm($form_state);

  switch ($form_state['values']['action']) {
    case 'add':
      // Create an array of the item to save.
      $cart_item = array(
        'uid' => $GLOBALS['user']->uid,
        'alma_id' => $form_state['values']['object']->data->localId,
        'ting_id' => $form_state['values']['object']->id,
        'created' => $_SERVER['REQUEST_TIME'],
        'title' => $form_state['values']['object']->data->title[0],
        'author' => $form_state['values']['object']->data->creator[0],
        'thumbnail_url' => $form_state['values']['object']->additionalInformation->thumbnailUrl,
      );

      // Save the item to the cart table.
      drupal_write_record('alma_cart', $cart_item);
      break;
  }

  $form_state['redirect'] = $form_state['values']['dest_url'];
}

