<?php

require_once(dirname(__FILE__).'/ting-dbc-php5-client/lib/TingClient.php');
require_once(dirname(__FILE__).'/ting-dbc-php5-client/lib/search/TingClientSearchRequest.php');
require_once(dirname(__FILE__).'/ting-dbc-php5-client/lib/adapter/request/http/TingClientDrupal6HttpRequestAdapter.php');
require_once(dirname(__FILE__).'/ting-dbc-php5-client/lib/adapter/request/http/TingClientHttpRequestFactory.php');
require_once(dirname(__FILE__).'/ting-dbc-php5-client/lib/adapter/response/json/TingClientJsonResponseAdapter.php');
require_once(dirname(__FILE__).'/ting-dbc-php5-client/lib/log/TingClientDrupalWatchDogLogger.php');

class TingClientFactory {
	/**
	 * @var TingClient
	 */
	private static $instance;
	
	/**
	 * @return TingClient
	 */
	public static function getClient() {
		if (!isset(self::$instance))
		{
			$baseUrl = variable_get('ting_server');
			if (!$baseUrl) {
				throw new TingClientException('No Ting server defined');
			}
			//Create client with default configuration: Drupal for requests and logging, json for format.
			self::$instance = new TingClient(new TingClientDrupal6HttpRequestAdapter(new TingClientHttpRequestFactory($baseUrl)),
																				new TingClientJsonResponseAdapter(),
																				new TingClientDrupalWatchDogLogger());
		}
		return self::$instance;
	}
	
	/**
	 * @param string $query
	 * @return TingClientSearchRequest
	 */
	public static function getSearchRequest($query) {
		$searchRequest = new TingClientSearchRequest($query);
		$searchRequest->setOutput('json'); //use json format per default
		$searchRequest->setStart(1);
		$searchRequest->setNumResults(10);
		$searchRequest->setFacets(array('dc.subject', 'dc.date', 'dc.type', 'dc.creator', 'dc.language'));
		$searchRequest->setNumFacets(10);
		return $searchRequest;
	}
		
}