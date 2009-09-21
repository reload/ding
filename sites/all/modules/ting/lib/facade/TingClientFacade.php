<?php

$includes = array('ting-dbc-php5-client/lib/TingClient',
									'ting-dbc-php5-client/lib/adapter/http/TingClientDrupal6HttpRequestAdapter',
									'ting-dbc-php5-client/lib/request/rest-json/RestJsonTingClientRequestFactory',
									'ting-dbc-php5-client/lib/log/TingClientDrupalWatchDogLogger',
									'addi-client/AdditionalInformationService');
foreach ($includes as $include)
{
	module_load_include('php', 'ting', 'lib/'.$include);
}

class TingClientFacade {
	
	/**
	 * @var TingClient
	 */
	private static $client;

	/**
	 * @var TingClientRequestFactory
	 */
	private static $requestFactory;	
	
	/**
	 * @return TingClient
	 */
	private static function getClient() {
		if (!isset(self::$client))
		{
			self::$client = new TingClient(new TingClientDrupal6HttpRequestAdapter(),
																				new TingClientDrupalWatchDogLogger());
		}
		return self::$client;
	}
	
	/**
	 * @return TingClientRequestFactory
	 */
	private static function getRequestFactory()
	{
		if (!isset(self::$requestFactory))
		{
			$urlVariables = array(	'search' => 'ting_search_url',
														 	'scan' => 'ting_scan_url',
															'object' => 'ting_search_url',
															'collection' => 'ting_search_url',
															'spell' => 'ting_spell_url',
															'recommendation' => 'ting_recommendation_server');
			
			$urls = array();
			foreach ($urlVariables as $name => $setting)
			{
				$urls[$name] = variable_get($setting, false);
				if (!$urls[$name]) {
					throw new TingClientException('No Ting webservice url defined for '.$name);
				}
			}
			
			self::$requestFactory = new RestJsonTingClientRequestFactory($urls);
		}
		return self::$requestFactory;
	}
	
	/**
	 * @param string $query
	 * @return TingClientSearchResult
	 */
	public static function search($query, $page = 1, $resultsPerPage = 10, $options = array()) {
		$request = self::getRequestFactory()->getSearchRequest();
		$request->setQuery($query);
		$request->setStart($resultsPerPage * ($page - 1) + 1);
		$request->setNumResults($resultsPerPage);
		
		$request->setFacets((isset($options['facets'])) ? $options['facets'] : array('facet.subject', 'facet.creator', 'dc.type', 'facet.date', 'facet.language'));
		$request->setNumFacets((isset($options['numFacets'])) ? $options['numFacets'] : ((sizeof($request->getFacets()) == 0) ? 0 : 10));
		
		$searchResult = self::getClient()->execute($request);
		
		
		//Decorate search result with additional information
		foreach ($searchResult->collections as &$collection)
		{
			$collection = self::addCollectionInfo($collection);
			$collection = self::addAdditionalInfo($collection);
			$collection = self::sortObjects($collection);
		}
		
		return $searchResult;
	}
	
	/**
	 * @param string $query The prefix to scan for
	 * @param int $numResults The numver of results to return
	 * @return TingClientScanResult
	 */
	public static function scan($query, $numResults = 10)
	{
		$request = self::getRequestFactory()->getScanRequest();
		$request->setField('phrase.anyIndexes');
		$request->setLower($query);
		$request->setNumResults($numResults);
		return self::getClient()->execute($request);
	}
	
	/**
	 * @param string $objectId
	 * @return TingClientObjectCollection
	 */
	public static function getCollection($objectId)
	{
		$request = self::getRequestFactory()->getCollectionRequest();
		$request->setObjectId($objectId);
		$collection = self::getClient()->execute($request);
		return self::addCollectionInfo(self::addAdditionalInfo($collection));
	}
	
	/**
	 * @param string $objectId
	 * @return TingClientObject
	 */
	public static function getObject($objectId)
	{
		$request = self::getRequestFactory()->getObjectRequest();
		$request->setObjectId($objectId);
		$object = self::getClient()->execute($request);
		return array_shift(self::addAdditionalInfo(array($object)));
	}

	/**
	 * @param string $localId
	 * @return TingClientObject
	 */
	public static function getObjectByLocalId($localId)
	{
		$request = self::getRequestFactory()->getObjectRequest();
		$request->setLocalId($localId);
		$object = self::getClient()->execute($request);
		return array_shift(self::addAdditionalInfo(array($object)));
	}	
	
	public static function getSpellSuggestions($word, $numResults = 10)
	{
		$request = self::getRequestFactory()->getSpellRequest();
		$request->setWord($word);
		$request->setNumResults($numResults);
		return self::getClient()->execute($request);
	}
	
	private static function addCollectionInfo(TingClientObjectCollection $collection)
	{
		$collection->url = url('ting/collection/' . $collection->objects[0]->id, array('absolute' => true));
		$types = array();
		foreach ($collection->objects as &$object)
		{
			$types = array_merge($types, $object->data->type);
		}
		$collection->types = array_unique($types);
		return $collection;
	}
	
	private static function sortObjects($collection)
	{
		usort($collection->objects, array('TingClientFacade', 'compareObjects'));
		$collection->objects = array_reverse($collection->objects);
		
		foreach ($collection->objects as $i => $object)
		{
			if (isset($object->additionalInformation->detailUrl) || 
					isset($object->additionalInformation->thumbnailUrl))
			{
				$object = array_shift(array_slice($collection->objects, $i, 1));
				unset($collection->objects[$i]);
				array_unshift($collection->objects, $object);
				break;
			}
		}
		
		return $collection;
	}
	
	private static function addObjectUrl(TingClientObject $object)
	{
		$object->url = url('ting/object/'.$object->id, array('absolute' => true));
		return $object;
	}
	
	private static function addAdditionalInfo($collection)
	{
		//Add additional information info for cover images
		$isbns = array();

		$addiVariables = array(	'wsdlUrl' => 'addi_wdsl_url',
														'username' => 'addi_username',
														'group' => 'addi_group',
														'password' => 'addi_password');
		foreach ($addiVariables as $name => &$setting)
		{
			$setting = variable_get($setting, false);
			if (!$name)
			{
				watchdog('TingClient', 'Additional Information service setting '.$name.' not set', array(), WATCHDOG_WARNING);
				return $collection;
			}
		}
		extract ($addiVariables);
		$additionalInformationService = new AdditionalInformationService($wsdlUrl, $username, $group, $password);
		
		$objects = (isset($collection->objects)) ? $collection->objects : $collection;
		
		foreach($objects as $object)
		{
			$object = self::addObjectUrl($object);
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
	
	private static function compareObjects($o1, $o2, $criterias = NULL)
	{
		$criterias = ($criterias) ? $criterias : array(	'language' => array('Dansk', 'Engelsk'),
																										'type' => array('Bog', 'DVD'));

		foreach ($criterias as $attribute => $values)
		{
			foreach ($values as $value)
			{
				$o1Check = in_array($value, $o1->data->$attribute);
				$o2Check = in_array($value, $o2->data->$attribute);
				if ($o1Check && !$o2Check)
				{
					return 1;
				} 
				else if (!$o1Check && $o2Check)
				{
					return -1;	
				}
				else if ($o1Check && $o2Check && (sizeof($criterias) > 1))
				{
					array_shift($criterias);
					return self::compareObjects($o1, $o2, $criterias);
				}
			}
		}
				
		return 0;
	}
		
}

?>
