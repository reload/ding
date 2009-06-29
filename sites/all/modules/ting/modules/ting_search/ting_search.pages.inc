<?php


function ting_search_pages_ting() {
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
 * Content search AJAX page callback.
 */
function ting_search_pages_content() {
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
