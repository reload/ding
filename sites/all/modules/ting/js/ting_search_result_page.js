// $Id$

/**
 * JavaScript helper for the Ting search result page.
 */

Drupal.behaviors.tingSearchResultPage = function () {
  // Put the search keys into the main searchbox.
  $("#block-ting-searchbox :text").val(Drupal.settings.tingSearch.keys);

  // Remove the original output from the search module, and make room
  // for our search results and the facet browser.
  $("#content-inner").html('<div id="ding-facet-browser"></div><div id="ting-search-result">Søger…</div>');

  // Search Ting via Kaspers methods.
  $.getJSON(Drupal.settings.tingSearch.url, {'query': 'dc.title:' + Drupal.settings.tingSearch.keys}, function (data) {
    Drupal.dingTingFacetBrowser("#ding-facet-browser", data);

  });
}

