// $Id$

/**
 * JavaScript helper for the Ting search result page.
 */

$(document).ready(function() {
  Drupal.tingSearch = {};

  // Put the search keys into the main searchbox.
  $("#block-ting_search-searchbox :text").val(Drupal.settings.tingSearch.keys);

  // Remove the original output from the search module, and make room
  // for our search results and the facet browser.
  $("#content-inner").html(Drupal.settings.tingSearch.result_template).find('#content-result, .count').hide();

  // Search Ting
  $.getJSON(Drupal.settings.tingSearch.ting_url, {'query': 'dc.title:' + Drupal.settings.tingSearch.keys}, function (data) {
    Drupal.tingResult("#ting-search-result", data);
    Drupal.tingFacetBrowser("#ting-facet-browser", "#ting-search-result", data);
    var tab = $("#ting-search-tabs li.ting");
    if (data.numTotalRecords) {
    	tab.addClass('active');
    	tab.find('.count-value').text(data.numTotalRecords);
			tab.find('.count').show();
			
      tab.html('<a href="#">' + tab.html() + '</a>');
      tab.click(function (eventObject) {
      	$(this).addClass('active').siblings().removeClass('active');
        $("#content-result").hide();
        $("#ting-result").show();
        return false;
      });
    }
  });

  // Search Drupals content.
  $.getJSON(Drupal.settings.tingSearch.content_url, {'query': Drupal.settings.tingSearch.keys}, function (data) {
    Drupal.tingSearch.contentData = data;
    var tab = $("#ting-search-tabs li.content");
    if (data.count) {
    	tab.find('.count-value').text(data.count);
			tab.find('.count').show();

      $("#content-result").html(Drupal.tingSearch.contentData.result_html);
      tab.click(function () {
      	$(this).addClass('active').siblings().removeClass('active');
        $("#ting-result").hide();
        $("#content-result").show();
        return false;
      });
    }
  });
});

