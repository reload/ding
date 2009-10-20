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
 * Redirec to object view based on local ID (used by bibliotek.dk).
 */
function ting_search_pages_object_redirect($local_id) {
  module_load_include('client.inc', 'ting');

  $object = ting_get_object_by_local_id($local_id);
  if (!$object) {
    drupal_not_found();
    exit;
  }
  drupal_goto($object->url);
}

/**
 * Return the URL of a photo of the object.
 */
function ting_search_cover($preset, $object_id) {
  module_load_include('client.inc', 'ting');

  // $path is without the file_directory_path() prefix.
  $path = ting_search_cover_path($object_id);

  if (!file_exists(file_create_path($path))) {
  	$object = ting_get_object_by_id($object_id, true);
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
  module_load_include('client.inc', 'ting');
  
  //Get users base query
  $query = $_REQUEST['query'];
  if (!$query)
  {
    drupal_not_found();
    exit;
  }

  //Extend query with selected facets
  if (isset($_GET['facets']) && $_GET['facets']) {
    $facetArray = array();

    $facets = explode(';', $_GET['facets']);
    foreach ($facets as $facet) {
      $facet = explode(':', $facet, 2);
      if ($facet[0]) {
        $facetArray[] = $facet[0].':"'.$facet[1].'"';
      }
    }

    $query .= ' AND '.implode(' AND ', $facetArray);
  }

  try {
    $page = isset($_REQUEST['page']) ? intval($_REQUEST['page']) : 1;
    $resultsPerPage = 10;
    $searchResult = ting_do_search($query, $page, $resultsPerPage, array('enrich' => true));
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
