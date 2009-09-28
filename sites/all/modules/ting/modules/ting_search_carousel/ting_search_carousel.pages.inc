<?php
// $Id$

/**
 * JSON callback to Drupal's content search.
 */
function ting_search_carousel_result_js() {
  $result = array(
    'count' => 0,
  );
  if (isset($_REQUEST['query'])) {
    // Drupal's do_search() is hard-coded to 10 results per page.
    $resultsPerPage = 10;
    // Identifier used by pager_query().
    $element = 0;
    $query = trim($_REQUEST['query']);

    $searchResult = do_search($query, 'node');

    $result = array(
      // do_search() calls pager_query() that populates these globals.
      'count' => $searchResult ? $GLOBALS['pager_total_items'][$element] : 0,
      // pager_query() page numbers are zero-based.
      'page' => $searchResult ? $GLOBALS['pager_page_array'][$element] + 1 : 0,
      'resultsPerPage' => $resultsPerPage,
      'result_html' => '',
    );
    foreach ($searchResult as $item) {
      if ($item->type == 'node') {
        $node = node_load($item->sid);
        $result['result_html'] .= node_view($node, TRUE);
      }
    }
    if ($searchResult) {
      $result['result_html'] .= theme('pager', NULL, $resultsPerPage, $element);
    }
  }

  drupal_json($result);
  exit;
}