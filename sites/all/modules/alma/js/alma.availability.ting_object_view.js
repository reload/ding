/**
 * @file
 * Specific availability behavior for detailed object page.
 */

Drupal.behaviors.almaAvailabilityTingObjectView = function () {
  // Use get_details to load detailed data for each item on the page.
  Drupal.almaAvailability.get_details(function (data, textStatus) {
    // Inject data into the list of library that has the item available. 
    if ($("#ting-object .alma-availability").length > 0) {
      $("#ting-object .alma-availability ul.library-list").empty();
      // Iterate over each Alma item's data.
      $.each(data, function (dataIndex, dataItem) {
        var container = $('#ting-item-' + dataItem.alma_id + ' .alma-availability ul.library-list');
        // Add a list item for each holding.
        $.each(this.holdings, function (holdingIndex, holdingData) {
          container.append('<li>' + Drupal.settings.alma.branches[holdingData.branch_id] + '</li>');
        });
        if (dataItem.available_count > 0) {
          $('#ting-item-' + dataItem.alma_id)
            .find('.alma-status')
              .addClass('available')
              .removeClass('waiting')
              .text(Drupal.t('available'))
        }
        else {
          $('#ting-item-' + dataItem.alma_id)
            .find('.alma-status')
              .addClass('unavailable')
              .removeClass('waiting')
              .text(Drupal.t('unavailable'))
            .end()
        }
      });
    }
  });
}

