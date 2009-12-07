/**
 * Availability information for all pages.
 *
 * Try to find Ting items and stuff availability data into them.
 */
Drupal.behaviors.almaAvailabilityGenericView = function () {
  Drupal.almaAvailability.get_details(function (data, textStatus) {
    $.each(data, function(itemId, itemData) {
      var $item = $('#ting-item-' + itemId);
      if (itemData.show_reservation_button && itemData.available_count > 0) {
        $item.find('.alma-status')
          .addClass('available')
          .removeClass('waiting')
          .text(Drupal.t('available'));
      }
      else {
        $item.find('.alma-status')
          .addClass('unavailable')
          .removeClass('waiting')
          .text(Drupal.t('unavailable'))
        .end()
        .find('ul.alma-cart-buttons > li > a')
          .addClass('disabled')
          // Remove all click handlers.
          .unbind('click');
      }
    });
  });
}

