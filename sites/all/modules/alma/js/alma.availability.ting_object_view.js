/**
 * @file
 * Specific availability behavior for detailed object page.
 */

Drupal.behaviors.almaAvailabilityTingObjectView = function () {
  // Use get_details to load detailed data for each item on the page.
  Drupal.almaAvailability.get_details(function (data, textStatus) {
    // Update the standard status messages.
    Drupal.almaAvailability.updateStatus(data);

    // Inject data into the list of library that has the item available. 
    if ($("#ting-object .alma-availability").length > 0) {
      $("#ting-object .alma-availability ul.library-list").empty();
      // Iterate over each Alma item's data.
      $.each(data, function (dataIndex, dataItem) {
        var container = $('#ting-item-' + dataItem.alma_id + ' .alma-availability ul.library-list');
        var uniqueHoldings = {}

        // Find holdings, unique by library name.
        $.each(this.holdings, function (holdingIndex, holdingData) {
          uniqueHoldings[holdingData.branch_id] = Drupal.settings.alma.branches[holdingData.branch_id];
        });

        // Add a list item for each holding.
        $.each(uniqueHoldings, function (branchID, branchName) {
          container.append('<li>' + branchName  + '</li>');
        });
      });
    }
  });
}

