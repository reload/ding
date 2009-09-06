/* $Id$ */

/**
 * @file ting_search.js
 * JavaScript file holding most of the ting search related functions.
 */

/**
 * Set up the results page.
 *
 * This is _not_ a Drupal.behavior, since those take a lot longer to load.
 */
jQuery(function($) {$(function(){
  $("#search :text")
    // Put the search keys into the main searchbox.
    .val(Drupal.settings.tingSearch.keys)
    // And trigger the change event so that InFieldLabes will work correctly.
    .change();

  // Configure our tabs
  $("#ting-search-tabs")
    .tabs()
    // Disable the website tab until we have some results.
    .tabs("disable", 1);

  Drupal.tingSearch.getTingData(Drupal.settings.tingSearch.ting_url,
                                Drupal.settings.tingSearch.keys);

  Drupal.tingSearch.getContentData(Drupal.settings.tingSearch.content_url,
                                   Drupal.settings.tingSearch.keys);
})});

// Container object
Drupal.tingSearch = {
  // Holds the number of results for each of the different types of search.
  resultCount: {}
};

// Get search data from Ting
Drupal.tingSearch.getTingData = function(url, keys) {
  $.getJSON(url, {'query': keys}, function (result) {
    if (result.count > 0) {
      Drupal.tingSearch.resultCount.ting = result.count;
      $("#ting-search-spinner").hide("normal");

      // Add the template for ting result and facet browser.
      $("#ting-search-placeholder").replaceWith(Drupal.settings.tingSearch.result_template);

      // Pass the data on to the result and facet browser handlers.
      Drupal.tingResult("#ting-search-result", "#ting-facet-browser", result);
      Drupal.tingFacetBrowser("#ting-facet-browser", "#ting-search-result", result);
    }
    else {
      Drupal.tingSearch.resultCount.ting = 0;
    }
    Drupal.tingSearch.updateTabs("ting");
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
    Drupal.tingSearch.updateTabs("content");

    if (data.count) {
      $("#content-result").html(Drupal.tingSearch.contentData.result_html);
      // Redo the click event bindings for the contentPager, since we'll
      // have a new pager from the result HTML.
      Drupal.tingSearch.contentPager();

      // If the show parameter is specified, show our results.
      if (show) {
        $("#content-result").show("fast");
      }
    }
  });
}

// Redirect clicks on the pager to reload the content search.
Drupal.tingSearch.contentPager = function() {
  $("#content-result .pager a").click(function (eventObject) {
    $("#content-result").hide("fast");
    $("#ting-search-spinner").show("normal");
    Drupal.tingSearch.getContentData(this.href, false, true);
    return false;
  });
}

// Helper function to update the state of our tabs.
Drupal.tingSearch.updateTabs = function (sender) {
  if (Drupal.tingSearch.resultCount.hasOwnProperty(sender)) {
    var tab = $('#ting-search-tabs li.' + sender);
    var count = Drupal.tingSearch.resultCount[sender];
    if (count == 0) {
      // For no results, replace the contents of the results container
      // with the no results message.
      $("#" + sender + "-result").html('<h4>' + Drupal.settings.tingResult.noResultsHeader + '</h4><p>' + Drupal.settings.tingResult.noResultsText + '</p>');
    }
    else if (sender == 'content') {
      // If we have a non-zero result count for content, enable its tab.
      $("#ting-search-tabs").tabs("enable", 1);
    }
    tab.removeClass('spinning');

    if (tab.find('span.count').length) {
      tab.find('span.count em').text(count);
    }
    else {
      tab.find('a').append(' <span class="count">(<em>' + count + '</em>)</span>');
    }
  }

  if (Drupal.tingSearch.resultCount.hasOwnProperty("ting") && Drupal.tingSearch.resultCount.hasOwnProperty("content")) {
    // When both searches has returned, make sure that we're in a
    // reasonably consistent state.
    $("#ting-search-spinner").hide("normal");

    // If there were no results from Ting and website results available,
    // switch to the website tab and diable the Ting tab.
    if (Drupal.tingSearch.resultCount.ting == 0 && Drupal.tingSearch.resultCount.content > 0) {
      $("#ting-search-tabs")
        .tabs("select", 1)
        .tabs("disable", 0);
    }
  }
}

