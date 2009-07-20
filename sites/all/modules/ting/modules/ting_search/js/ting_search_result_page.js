// $Id$

/**
 * JavaScript helper for the Ting search result page.
 */



// Failsafe document.ready().
jQuery(function($) {$(function(){

  // Put the search keys into the main searchbox.
  $("#block-ting_search-searchbox :text").val(Drupal.settings.tingSearch.keys);

  // Remove the original output from the search module, and make room
  // for our search results and the facet browser.
  $("#ting-search-placeholder").replaceWith(Drupal.settings.tingSearch.result_template)
  // Hide the result count until the search results are loadad via JSON
  // callbacks to Drupal.
  $('#ting-search-tabs .count').hide();

  Drupal.tingSearch.getTingData(Drupal.settings.tingSearch.ting_url,
                                Drupal.settings.tingSearch.keys)


  Drupal.tingSearch.getContentData(Drupal.settings.tingSearch.content_url,
                                   Drupal.settings.tingSearch.keys)


});});

