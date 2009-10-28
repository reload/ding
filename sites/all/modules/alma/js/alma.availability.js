// $Id$
/**
 * @file alma.availability.js
 * JavaScript behaviours for fetching and displaying availability.
 */

Drupal.behaviors.almaAvailability = function () {
  var items = $("div.ting-item");
  var id_matcher = /ting-item-(\d+)/;
  var id_list = [];

  items.each(function (i) {
    id_list.push(id_matcher.exec(this.id)[1]);
  });

  $.getJSON(Drupal.settings.alma.base_url + '/item/' + id_list.join('.') + '/status', {}, function (data, textStatus) {
    if ($("#ting-object .alma-availability").length == 1) {
      $("#ting-object .alma-availability ul.library-list").empty();
      // Iterate over each Alma item's data.
      $.each(data, function (dataIndex, dataItem) {
        var container = $('#ting-item-' + dataItem.alma_id + ' .alma-availability ul.library-list');
        // Add a list item for each holding.
        $.each(this.holdings, function (holdingIndex, holdingData) {
          container.append('<li>' + Drupal.settings.alma.branches[holdingData.branch_id] + '</li>');
        });
      });
    }
  });
}

