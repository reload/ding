<?php
// $Id$

/**
 * Theme function to render reservation buttons for a TingClientObject.
 *
 * @param TingClientObject $object
 * @return string
 */
function theme_alma_cart_reservation_buttons($object) {
  if (isset($object->data->localId) && !empty($object->data->localId)) {
    $output = theme('item_list', array(
      l(t('Reserve now'), 'alma/cart/reserve/' . $object->data->localId, array('attributes' => array('class' => 'reserve-now'))),
      l(t('Add to cart'), 'alma/cart/add/'  . $object->data->localId, array('attributes' => array('class' => 'add-to-cart'))),
    ), NULL, 'ul', array('class' => 'buttons'));
    return $output;
  }
}

