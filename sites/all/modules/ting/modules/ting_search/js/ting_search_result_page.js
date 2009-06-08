// $Id$

/**
 * JavaScript helper for the Ting search result page.
 */

$(document).ready(function() {
  Drupal.tingSearch = {};

  // Put the search keys into the main searchbox.
  $("#block-ting-searchbox :text").val(Drupal.settings.tingSearch.keys);

  // Remove the original output from the search module, and make room
  // for our search results and the facet browser.
  $("#content-inner").html('<ul id="ting-search-tabs"><li class="ting">Ting</li><li class="content">Content</li></ul><div id="ding-facet-browser"></div><div id="ting-search-result">Søger…</div>').end();

  // Search Ting via Kaspers methods.
  $.getJSON(Drupal.settings.tingSearch.ting_url, {'query': 'dc.title:' + Drupal.settings.tingSearch.keys}, function (data) {
    Drupal.tingSearch.tingData = data;
    Drupal.dingTingFacetBrowser("#ding-facet-browser", data);
  });

  // Search Drupals content.
  $.getJSON(Drupal.settings.tingSearch.content_url, {'query': Drupal.settings.tingSearch.keys}, function (data) {
    Drupal.tingSearch.contentData = data;
    var tab = $("#ting-search-tabs li.content");
    tab.append(' <span>(' + data.count + ')</span>');
    if (data.count) {
      tab.html('<a href="#">' + tab.html() + '</a>');
      tab.click(function (eventObject) {
        $("#ding-facet-browser").hide();
        $("#ting-search-result").html(Drupal.tingSearch.contentData.result_html);
        return false;
      });
    }
  });
});

