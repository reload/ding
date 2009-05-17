<?php

require_once dirname(__FILE__).'/lib/DingTingClientFactory.php';

function ding_ting_search_result_page()
{
	$query = (isset($_REQUEST['query'])) ? $_REQUEST['query'] : 'dc.title:danmark';
	$request = DingTingClientFactory::getSearchRequest($query);
	
	$client = DingTingClientFactory::getClient();
	$result = $client->search($request);
	return theme('ding_ting_search_result', $result);
}

function ding_ting_ajax_search_result_page()
{
	$output = '';
	return $output;
}