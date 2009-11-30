// $Id$

/**
 * @file
 * JavaScript behavior to update library open/closed status dynamically.
 */

Drupal.behaviors.dingLibraryStatus = function () {
  $.getJSON(Drupal.settings.officeHours[1].callback + '/' + Drupal.settings.dingLibraryNids.join(',') + '/' + Drupal.settings.officeHours[1].field_name, {}, function (response, textStatus) {
    $.each(response.data, function (nid, hoursData) {
      $('#node-' + nid + ' .libary-openstatus')
        .text(hoursData.status_local);
    });
  });
};

// Object to hold our globally accessible stuff.
Drupal.dingLibraryStatus = {
  // Refresh library status every 5 minutes.
  'interval': setTimeout(Drupal.behaviors.dingLibraryStatus, 300000)
}

