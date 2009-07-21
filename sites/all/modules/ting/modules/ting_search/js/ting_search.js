/* $Id$ */

/**
 * @file ting_search.js
 * JavaScript file holding most of the ting search related functions.
 */

// Container object
Drupal.tingSearch = {
  // Holds the number of results for each of the different types of search.
  resultCount: {}
};

// Get search data from Ting
Drupal.tingSearch.getTingData = function(url, keys) {
  $.getJSON(url, {'query': 'dc.title:' + keys}, function (data) {
    Drupal.tingSearch.resultCount.ting = data.numTotalRecords;
    if (data.numTotalRecords > 0) {
      // Pass the data on to the result and facet browser handlers.
      Drupal.tingResult("#ting-search-result", "#ting-facet-browser", data);
      Drupal.tingFacetBrowser("#ting-facet-browser", "#ting-search-result", data);
      // Hide the spinner, since we're finished loading.
      $("#ting-search-spinner").hide("normal");

      // Update and show the result count on the tab and make it clickable.
      $("#ting-search-tabs li.ting")
        .addClass('active')
        .find('.count-value').text(data.numTotalRecords).end()
        .find('.count').show().end()
        .append('<a href="#">')
        .prepend('</a>')
        .click(function (eventObject) {
          $(this).addClass('active').siblings().removeClass('active');
          $("#content-result").hide();
          $("#ting-result").show();
          return false;
        });
    }
  });
}

// Get search data from Drupal's content search
Drupal.tingSearch.getContentData = function(url, keys, show) {
  // Set up a params object to send along to getJSON.
  var params = {};
  // If we get new search keys via the keys parameter, they'll get
  // attached to the object here
  if (keys) {
    params.query = keys
  }

  $.getJSON(url, params, function (data) {
    // Store some of the data returned on the tingSearch object in case
    // we should need it later.
    Drupal.tingSearch.resultCount.content = data.count;
    Drupal.tingSearch.contentData = data;

    if (data.count) {
      $("#content-result").html(Drupal.tingSearch.contentData.result_html);
      // Redo the click event bindings for the contentPager, since we'll
      // have a new pager from the result HTML.
      Drupal.tingSearch.contentPager();

      // If the show parameter is specified, show our results.
      if (show) {
        $("#content-result").show("fast");
      }

      // Hide the spinner, since we're finished loading.
      $("#ting-search-spinner").hide("normal");

      // Update and show the result count on the tab and make it clickable.
      $("#ting-search-tabs li.content")
        .find(".count-value").text(data.count).end()
        .find(".count").show().end()
        .append('<a href="#">')
        .prepend('</a>')
        .click(function () {
          $(this).addClass('active').siblings().removeClass('active');
          $("#ting-result").hide();
          $("#content-result").show();
          return false;
        });
    }
  });
}

// Redirect clicks on the pager to reload the content search.
Drupal.tingSearch.contentPager = function() {
  $("#content-result .pager a").click(function (eventObject) {
    $("#content-result").hide("fast");
    $("#ting-search-spinner").show("normal");
    Drupal.tingSearch.getContentData(eventObject.srcElement.href, false, true);
    return false;
  });
}

