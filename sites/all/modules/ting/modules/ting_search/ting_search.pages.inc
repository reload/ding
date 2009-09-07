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
 * Display a Ting collection of objects.
 */
function ting_collection($object_id) {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');

  $facade = new TingClientFacade();
  
  $collection = $facade->getCollection($object_id);
  if (!$collection) {
    return drupal_not_found();
  }

  return theme('ting_collection', $collection);
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
    $searchResult = $facade->search($query, isset($_REQUEST['page']) ? $_REQUEST['page'] : 1, 10);
    $result = array(
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
    $query = trim($_REQUEST['query']);
    $result = array('results' => do_search($query, 'node'));
    $result['count'] = count($result['results']);
    $result['result_html'] = '';
    foreach ($result['results'] as $item) {
      if ($item->type == 'node') {
        $node = node_load($item->sid);
        $result['result_html'] .= node_view($node, TRUE);
      }
    }
    if (!empty($result['result_html'])) {
      $result['result_html'] .= theme('pager', NULL, 10, 0);
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
