<?php
// $Id$

/**
 * Rendering of Alma cart item.
 */
function theme_alma_cart_item($item) {
  $output = theme('image', $item['thumbnail_url'], '', $item['title'], array('class' => 'cart-item-thumbnail'), FALSE);
  $output .= '<span class="title">' . $item['title'] . '</span> ';
  $output .= t('by');
  $output .= ' <span class="author">' . $item['author'] . '</span>';
  $output .= ' <span class="type">' . $item['type'] . '</span>';
  return $output;
}

/**
 * Theme function to render reservation buttons for a TingClientObject.
 *
 * @param TingClientObject $object
 * @return string
 */
function theme_alma_cart_reservation_buttons($object) {
  if (isset($object->data->localId) && !empty($object->data->localId)) {
    // TODO: Check if item is reserved/in basket already.
    $output = theme('item_list', array(
      l(t('Reserve now'), 'alma/cart/reserve/' . $object->data->localId, array('attributes' => array('class' => 'reserve-now'))),
      l(t('Add to cart'), 'alma/cart/add/'  . $object->data->localId, array('attributes' => array('class' => 'add-to-cart'))),
    ), NULL, 'ul', array('class' => 'buttons'));
    return $output;
  }
}

/**
 * Theme function for Alma cart form.
 */
function theme_alma_cart_forms_cart_details($form) {
  $header = array(t('Select'), t('Title'), t('Wait time'));
  $rows = array();
  $output = '';

  foreach ($form['cart_data']['#value'] as $item) {
    $rows[] = array(
      drupal_render($form['cart'][$item['alma_id']]),
      theme('alma_cart_item', $item),
      '~0.25E-3 millenia', // TODO: Wait time not implemented yet.
    );
  }

  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

