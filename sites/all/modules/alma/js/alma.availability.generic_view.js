/**
 * Availability information for all pages.
 *
 * Try to find Ting items and stuff availability data into them.
 */
Drupal.behaviors.almaAvailabilityGenericView = function () {
  Drupal.almaAvailability.get_availability(function (data, textStatus) {
    $.each(Drupal.almaAvailability.id_list, function(idIndex, idValue) {
      if (data[idValue]) {
        $('#ting-item-' + idValue)
          .find('.alma-status')
            .addClass('available')
            .removeClass('waiting')
            .text(Drupal.t('available'))
      }
      else {
        $('#ting-item-' + idValue)
          .find('.alma-status')
            .addClass('unavailable')
            .removeClass('waiting')
            .text(Drupal.t('unavailable'))
          .end()
      }
    });
  });
}

