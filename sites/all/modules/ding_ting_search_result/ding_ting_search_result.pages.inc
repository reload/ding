<?php

require_once dirname(__FILE__).'/lib/DingTingClientFactory.php';

function ding_ting_search_result_page()
{
	$query = (isset($_REQUEST['query'])) ? $_REQUEST['query'] : 'dc.title:danmark';
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