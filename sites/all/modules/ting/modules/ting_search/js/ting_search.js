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
    var tab = $("#ting-search-tabs li.ting");
    if (data.numTotalRecords > 0) {
      Drupal.tingResult("#ting-search-result", "#ting-facet-browser", data);
      Drupal.tingFacetBrowser("#ting-facet-browser", "#ting-search-result", data);
    	tab.addClass('active');
    	tab.find('.count-value').text(data.numTotalRecords);
			tab.find('.count').show();
			
      tab.html('<a href="#">' + tab.html() + '</a>');
      $("#ting-search-spinner").hide("normal");
      tab.click(function (eventObject) {
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
  var params = {};
  if (keys) {
    params.query = keys
  }
  $.getJSON(url, params, function (data) {
    Drupal.tingSearch.resultCount.content = data.count;
    Drupal.tingSearch.contentData = data;

    if (data.count) {
      $("#content-result").html(Drupal.tingSearch.contentData.result_html);
      Drupal.tingSearch.contentPager();
      if (show) {
        $("#content-result").show("fast");
      }
      $("#ting-search-spinner").hide("normal");

      // Update and show the result count on the tab and make it clickable.
      $("#ting-search-tabs li.content")
        .find(".count-value").text(data.count).end()
        .find(".count").show().end()
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

