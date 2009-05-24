<?php

require_once dirname(__FILE__).'/lib/DingTingClientFactory.php';

function ding_ting_search_result_page()
{
	//Get users base query
	$query = (isset($_REQUEST['query'])) ? $_REQUEST['query'] : 'dc.title:danmark';

	//Extend query with selected facets
	$facetArray = array();
	if (isset($_GET['facets']))
	{
		$facets = explode(';', $_GET['facets']);
		foreach ($facets as $facet)
		{
			$facet = explode(':', $facet, 2);
			if ($facet[0])
			{
				$facetArray[] = $facet[0].':'.$facet[1];
			}
		}	
	}
	$query .= ' and '.implode(' and ', $facetArray);
	
	$request = DingTingClientFactory::getSearchRequest($query);
	
	$client = DingTingClientFactory::getClient();
	$result = $client->search($request);
	
	if (ding_ting_search_result_is_ajax())
	{
		header('Content-type: application/json');
		echo json_encode($result);
		exit;
	} else {
		return theme('ding_ting_search_result', $result);
	}
}

function ding_ting_search_result_is_ajax() {
	return (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && 
					($_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest'));
}