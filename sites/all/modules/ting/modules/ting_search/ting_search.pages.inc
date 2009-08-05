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
 * JSON callback to search Ting.
 */
function ting_search_pages_ting_js() {
  module_load_include('php', 'ting', 'lib/TingClientFactory');
  module_load_include('php', 'ting', 'lib/services/AdditionalInformationService');
  
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
	
	$resultsPerPage = 10;
	$page = $_REQUEST['page'];
	$request->setStart($resultsPerPage * ($page - 1));	
	
	$client = TingClientFactory::getClient();
	$result = $client->search($request);

	//Add additional information info for cover images
	$isbns = array();
	foreach ($result->records as $record)
	{
		if (isset($record->data->identifier))
		{
			foreach ($record->data->identifier as $identifier)
			{
				if ($identifier->type == TingClientRecordIdentifier::ISBN)
				{
					$isbns[] = $identifier->id;
				}
			}
		}
	}

	if (sizeof($isbns) > 0)
	{
		//TODO: Move account information to admin settings page
		$additionalInformationService = new AdditionalInformationService('netpunkt', '710100', 'Juni1706');
		$additionalInformations = $additionalInformationService->getByIsbn($isbns);
		
		foreach ($additionalInformations as $id => $ai)
		{
			foreach ($result->records as &$record)
			{
				if (isset($record->data->identifier))
				{						
					foreach ($record->data->identifier as $identifier)
					{
						if ($identifier->type == TingClientRecordIdentifier::ISBN &&
								$identifier->id == $id)
						{	
							$record->additionalInformation = $ai;
						}
					}
				}
			}
		}
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
