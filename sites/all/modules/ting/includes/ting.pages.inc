<?php // $Id$

/**
 * @file ting.pages.inc
 * Page callbacks for the Ting module.
 */

/**
 * Content search AJAX page callback.
 */
function ting_pages_search_content() {
  if (isset($_REQUEST['query'])) {
    $query = trim($_REQUEST['query']);
    $result = array('results' => do_search($query, 'node'));
    $result['count'] = count($result['results']);
    $result['result_html'] = 'Bøf.';
    foreach ($result['results'] as $item) {
      if ($item->type == 'node') {
        $node = node_load($item->sid);
        $result['result_html'] .= node_view($node, TRUE);
      }
    }
  }
  else {
    $result = array(
      'results' => array(),
      'count' => 0,
      'result_html' => '',
    );
  }
  header('Content-type: application/json');
  print json_encode($result);
  exit;
}

