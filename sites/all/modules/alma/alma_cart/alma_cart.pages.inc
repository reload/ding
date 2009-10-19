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

  // If client wants JSON (usually AJAX requests), let's give it to them.
  if (isset($_SERVER['HTTP_ACCEPT']) && (strpos($_SERVER['HTTP_ACCEPT'], 'application/json')) || (strpos($_SERVER['HTTP_ACCEPT'], 'text/javascript'))) {
    $reply = array();
    if (!$GLOBALS['user']->uid) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'not-logged-in';
      $reply['title'] = t('Not logged in');
      $reply['message'] = t('You have to log in to put books in your basket');
    }
    elseif ($exists) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'already-in-basket';
      $reply['title'] = t('Already in basket');
      $reply['message'] = t('@item is already in your basket.', array('@item' => $object->data->title[0]));
    }
    else {
      if (alma_cart_add_ting_object($object) == SAVED_NEW) {
        $reply['status'] = 'success';
        $reply['title'] = t('Success');
        $reply['message'] = t('@item is now in your basket.', array('@item' => $object->data->title[0]));
        $reply['cart_link'] = url('user/' . $GLOBALS['user']->uid . '/cart');
      }
      else {
        $reply['status'] = 'error';
        $reply['error_type'] = 'cart-save-error';
        $reply['title'] = t('Cart saving error');
        $reply['message'] = t('An error occurred while saving to the basket. Please try again.');
      }
    }

    // We are returning JavaScript, so tell the browser.
    drupal_set_header('Content-Type: application/json; charset=utf-8');
    echo json_encode($reply);
    return;
  }

  // For regular POST requests, we present forms, etc.
  if ($exists) {
    drupal_set_message('%book is already in your cart.', array('%book' => $object->data->title[0]));
    return drupal_goto('user/' . $GLOBALS['user']->uid . '/cart');
  }

  // Present a confirmation form.
  return drupal_get_form('alma_cart_confirm_form', $object, 'add');

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
  switch ($form_state['values']['action']) {
    case 'add':
      // Save the item to the cart table.
      alma_cart_add_ting_object($form_state['values']['object']);
      break;
  }

  $form_state['redirect'] = $form_state['values']['dest_url'];
}

