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
  jquery_ui_add('ui.dialog');
  jquery_ui_theme_load();
  drupal_add_js(drupal_get_path('module', 'alma_cart') . '/alma_cart.buttons.js');
  if (isset($object->data->localId) && !empty($object->data->localId)) {
    // TODO: Check if item is reserved/in basket already.
    $output = theme('item_list', array(
      l(t('Reserve now'), 'alma/cart/reserve/' . $object->data->localId, array('attributes' => array('class' => 'reserve-now'))),
      l(t('Add to cart'), 'alma/cart/add/'  . $object->data->localId, array('attributes' => array('class' => 'add-to-cart'))),
    ), NULL, 'ul', array('class' => 'buttons alma-cart-buttons'));
    return $output;
  }
}

