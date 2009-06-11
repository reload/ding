<?php


function ting_search_pages_ting() {
  module_load_include('php', 'ting', 'lib/TingClientFactory');
  
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

	$request = TingClientFactory::getSearchRequest($query);

	$client = TingClientFactory::getClient();
	$result = $client->search($request);

  header('Content-type: application/json');
  echo json_encode($result);
  exit;
}

/**
 * Content search AJAX page callback.
 */
function ting_search_pages_content() {
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
