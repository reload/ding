<?php

require_once dirname(__FILE__).'/lib/TingClientFactory.php';

function ting_result_page()
{
	//Get users base query
	$query = (isset($_REQUEST['query'])) ? $_REQUEST['query'] : 'dc.title:danmark';

	//Extend query with selected facets
	if (isset($_GET['facets']) && $_GET['facets'])
	{
		$facetArray = array();

		$facets = explode(';', $_GET['facets']);
		foreach ($facets as $facet)
		{
			$facet = explode(':', $facet, 2);
			if ($facet[0])
			{
				$facetArray[] = $facet[0].':'.$facet[1];
			}
		}
			
		$query .= ' AND '.implode(' AND ', $facetArray);
	}

	$request = TingClientFactory::getSearchRequest($query);

	$client = TingClientFactory::getClient();
	$result = $client->search($request);

	if (ting_result_is_ajax())
	{
		header('Content-type: application/json');
		echo json_encode($result);
		exit;
	} else {
		return theme('ting_result', $result);
	}
}

function ting_result_is_ajax() {
	return (isset($_SERVER['HTTP_X_REQUESTED_WITH']) &&
	($_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest'));
}