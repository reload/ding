// $Id$
/**
 * @file alma.availability.js
 * JavaScript behaviours for fetching and displaying availability.
 */

// Container object for all our availability stuff.
Drupal.almaAvailability = {
  id_matcher: /ting-item-(\d+)/,
  id_list: []
}

/**
 * Helper function to find and store all ting item ids.
 */
Drupal.almaAvailability.find_ids = function () {
  $("div.ting-item").each(function () {
    Drupal.almaAvailability.id_list.push(Drupal.almaAvailability.id_matcher.exec(this.id)[1]);
  });
};

/**
 * Get details for all ting items found.
 */
Drupal.almaAvailability.get_details = function (callback) {
  // If the id_list is empty, try and find ids again.
  if (Drupal.almaAvailability.id_list.length == 0) {
    Drupal.almaAvailability.find_ids();
  }

  if (Drupal.almaAvailability.id_list.length > 0) {
    $.getJSON(Drupal.settings.alma.base_url + '/item/' + Drupal.almaAvailability.id_list.join(',') + '/status', {}, callback);
  }
}

