<?php

$includes = array('ting-dbc-php5-client/lib/TingClient',
									'ting-dbc-php5-client/lib/search/TingClientSearchRequest',
									'ting-dbc-php5-client/lib/adapter/request/http/TingClientDrupal6HttpRequestAdapter',
									'ting-dbc-php5-client/lib/adapter/request/http/TingClientHttpRequestFactory',
									'ting-dbc-php5-client/lib/adapter/response/json/TingClientJsonResponseAdapter',
									'ting-dbc-php5-client/lib/log/TingClientDrupalWatchDogLogger',
									'addi-client/AdditionalInformationService');
foreach ($includes as $include)
{
	module_load_include('php', 'ting', 'lib/'.$include);
}

class TingClientFacade {
	
	private static $format = 'json';
	
	/**
	 * @var TingClient
	 */
	private static $client;
	
	/**
	 * @return TingClient
	 */
	private static function getClient() {
		if (!isset(self::$client))
		{
			$baseUrl = variable_get('ting_server', false);
			if (!$baseUrl) {
				throw new TingClientException('No Ting server defined');
			}
			//Create client with default configuration: Drupal for requests and logging, json for format.
			self::$client = new TingClient(new TingClientDrupal6HttpRequestAdapter(new TingClientHttpRequestFactory($baseUrl)),
																				new TingClientJsonResponseAdapter(),
																				new TingClientDrupalWatchDogLogger());
		}
		return self::$client;
	}
	
	/**
	 * @param string $query
	 * @return TingClientSearchResult
	 */
	public static function search($query, $page = 1, $resultsPerPage = 10) {
		$searchRequest = new TingClientSearchRequest($query);
		$searchRequest->setOutput(self::$format); //use json format per default
		$searchRequest->setStart($resultsPerPage * ($page - 1));
		$searchRequest->setNumResults($resultsPerPage);
		
		$searchRequest->setFacets(array('dc.subject', 'dc.date', 'dc.type', 'dc.creator', 'dc.language'));
		$searchRequest->setNumFacets(10);
		
		$searchResult = self::getClient()->search($searchRequest);
		
		//Decorate search result with additional information
		foreach ($searchResult->collections as &$collection)
		{
			$collection = self::addCollectionUrls($collection);
			$collection = self::addAdditionalInfo($collection);
		}
		
		return $searchResult;
	}
	
	/**
	 * @param string $objectId
	 * @return TingClientObjectCollection
	 */
	public static function getCollection($objectId)
	{
		$collection = self::getClient()->getCollection(new TingClientCollectionRequest($objectId, self::$format));
		return self::addCollectionUrls(self::addAdditionalInfo($collection));
	}
	
	/**
	 * @param string $objectId
	 * @return TingClientObject
	 */
	public static function getObject($objectId)
	{
		return self::getClient()->getObject(new TingClientObjectRequest($objectId, self::$format));	
	}
	
	private static function addCollectionUrls(TingClientObjectCollection $collection)
	{
		$collection->url = url('ting/collection', array('query' => array('object_id' => $collection->objects[0]->data->title[0])));
		foreach ($collection->objects as &$object)
		{
			$object = self::addObjectUrl($object);
		}
		return $collection;
	}
	
	private static function addObjectUrl(TingClientObject $object)
	{
		$object->url = url('ting/object', array('query' => array('object_id' => $object->id)));
		return $object;
	}
	
	private static function addAdditionalInfo($collection)
	{
		//Add additional information info for cover images
		$isbns = array();
		
		$objects = (isset($collection->objects)) ? $collection->objects : $collection;
		
		foreach($objects as $object)
		{
			if (isset($object->data->identifier))
			{
				foreach ($object->data->identifier as $identifier)
				{
					if ($identifier->type == TingClientObjectIdentifier::ISBN)
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
				foreach ($objects as &$object)
				{
					if (isset($object->data->identifier))
					{						
						foreach ($object->data->identifier as $identifier)
						{
							if ($identifier->type == TingClientObjectIdentifier::ISBN &&
									$identifier->id == $id)
							{	
								$object->additionalInformation = $ai;
							}
						}
					}
				}
			}
		}
		
		if (isset($collection->objects))
		{
			$collection->objects = $objects;
		}
		else
		{
			$collection = $objects;			
		}
		
		return $collection;
	}
		
}

?>