<?php
// $Id$

/**
 * Search result page.
 */
function ting_search_pages_redirect() {
  if (isset($_POST['keys'])) {
    return drupal_goto('ting/search/' . $_POST['keys']);
  }

  $keys = search_get_keys();
  if (strlen($keys) > 0) {
    // Redirect to the new Panels based search.
    return drupal_goto('ting/search/' . $keys, NULL, NULL, 301);
  }
  return drupal_not_found();
}

/**
 * Return the URL of a photo of the object.
 */
function ting_search_cover($preset, $object_id) {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');

  // $path is without the file_directory_path() prefix.
  $path = ting_search_cover_path($object_id);

  if (!file_exists(file_create_path($path))) {
    $facade = new TingClientFacade();
  	$object = $facade->getObject($object_id);
    if (!$object) {
      drupal_not_found();
      exit;
    }

    if ($object->additionalInformation->detailUrl) {
      $source_url = $object->additionalInformation->detailUrl;
    }
    elseif ($object->additionalInformation->thumbnailUrl) {
      $source_url =$object->additionalInformation->thumbnailUrl;
    }
    else {
      drupal_not_found();
      exit;
    }

    $result = drupal_http_request($source_url);
    if ($result->code == 200) {
      file_check_directory(file_create_path(dirname($path)), FILE_CREATE_DIRECTORY);
      file_put_contents($path, $result->data);
      file_save_data($result->data, $path, FILE_EXISTS_REPLACE);
    }
  }

  drupal_goto(imagecache_create_url($preset, $path));
}

/**
 * JSON callback to search Ting.
 */
function ting_search_pages_ting_js() {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
  
  //Get users base query
  $query = (isset($_REQUEST['query'])) ? $_REQUEST['query'] : 'dc.title:danmark';

  //Extend query with selected facets
  if (isset($_GET['facets']) && $_GET['facets']) {
    $facetArray = array();

    $facets = explode(';', $_GET['facets']);
    foreach ($facets as $facet) {
      $facet = explode(':', $facet, 2);
      if ($facet[0]) {
        $facetArray[] = '('.$facet[0].':'.$facet[1].')';
      }
    }

    $query .= ' AND '.implode(' AND ', $facetArray);
  }

  $facade = new TingClientFacade();

  try {
    $page = isset($_REQUEST['page']) ? intval($_REQUEST['page']) : 1;
    $resultsPerPage = 10;
    $searchResult = $facade->search($query, $page, $resultsPerPage);
    $result = array(
      'page' => $page,
      'resultsPerPage' => $resultsPerPage,
      'count' => $searchResult->numTotalObjects,
      'result_html' => '',
      'facets' => $searchResult->facets,
    );
    foreach ($searchResult->collections as &$collection) {
      $result['result_html'] .= theme('ting_search_collection', $collection);
    }
  }
  catch (TingClientException $e) {
    // TODO: Log the error.
    $result = array('count' => 0);
  }

  drupal_json($result);
  exit;
}

/**
 * JSON callback to Drupal's content search.
 */
function ting_search_pages_content_js() {
  if (isset($_REQUEST['query'])) {
    // Drupal's do_search() is hard-coded to 10 results per page.
    $resultsPerPage = 10;
    // Identifier used by pager_query().
    $element = 0;
    $query = trim($_REQUEST['query']);
    $result = array('results' => do_search($query, 'node'));
    // do_search() calls pager_query() that populates these globals,
    $result['count'] = $GLOBALS['pager_total_items'][$element];
    // pager_query() page numbers are zero-based.
    $result['page'] = $GLOBALS['pager_page_array'][$element] + 1;
    $result['resultsPerPage'] = $resultsPerPage;
    $result['result_html'] = '';
    foreach ($result['results'] as $item) {
      if ($item->type == 'node') {
        $node = node_load($item->sid);
        $result['result_html'] .= node_view($node, TRUE);
      }
    }
    if (!empty($result['result_html'])) {
      $result['result_html'] .= theme('pager', NULL, $resultsPerPage, $element);
    }
  }
  else {
    $result = array(
      'results' => array(),
      'count' => 0,
      'result_html' => '',
    );
  }
  drupal_json($result);
  exit;
}
