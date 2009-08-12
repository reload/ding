<?php
// $Id$

/**
 * Search result page.
 */
function ting_search_pages_result() {
  $keys = search_get_keys();
  // Loads the JavaScript files.
  ting_search_add_js();

  drupal_add_js(array('tingSearch' => array(
    'result' => array(),
    'keys' => $keys,
    'ting_url' => url('ting/search/js'),
    'content_url' => url('ting/search/content/js'),
    'result_template' => theme('ting_result')
  )), 'setting', 'footer');
  return theme('ting_search_result_page', theme('ding_campaign_relevant_campaigns', (array('search_term' => $keys))));
}

/**
 * Display a Ting collection of objects.
 */
function ting_collection() {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
	
  $facade = new TingClientFacade();
  
	$collection = $facade->getCollection($_REQUEST['object_id']);
	if (!$collection)
	{
		drupal_not_found();
		exit;
	}
	
  return theme('ting_collection', $collection);
}

/**
 * Display a signle Ting object.
 */
function ting_object() {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
	
  $facade = new TingClientFacade();
  
	$object = $facade->getObject($_REQUEST['object_id']);
	if (!$object)
	{
		drupal_not_found();
		exit;
	}
	
  return theme('ting_object', $object);
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
				$facetArray[] = $facet[0].':'.$facet[1];
			}
		}
			
		$query .= ' AND '.implode(' AND ', $facetArray);
	}

	$facade = new TingClientFacade();
	
	$result = $facade->search($query, $_REQUEST['page'], $resultsPerPage);

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
