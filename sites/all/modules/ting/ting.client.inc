<?php
// $Id$

/**
 * @file ting.client.inc
 * Wrapper functions for Ting client.
 */

// Ting client has a ton of includes. Let's load them.
$includes = array(
  'ting-dbc-php5-client/lib/TingClient.php',
  'ting-dbc-php5-client/lib/adapter/http/TingClientDrupal6HttpRequestAdapter.php',
  'ting-dbc-php5-client/lib/request/rest-json/RestJsonTingClientRequestFactory.php',
  'ting-dbc-php5-client/lib/log/TingClientDrupalWatchDogLogger.php',
  'addi-client/AdditionalInformationService.php'
);

$path = drupal_get_path('module', 'ting') . '/lib/';

foreach ($includes as $include) {
  require_once($path . $include);
}

/**
 * Load an object from Ting by ID.
 *
 * @param string $object_id
 *    Ting object ID.
 * @return mixed
 *    Object, if found - boolean FALSE if not.
 */
function ting_get_object_by_id($object_id) {
	$request = ting_get_request_factory()->getObjectRequest();
	$request->setObjectId($object_id);
	$object = ting_get_client()->execute($request);
	return ($object) ? array_shift(ting_add_collection_info(array(ting_add_object_url($object)))) : false;
}

/**
 * @param string $localId
 * @return TingClientObject
 */
function ting_get_object_by_local_id($local_id) {
	$request = ting_get_request_factory()->getObjectRequest();
	$request->setLocalId($local_id);
	$object = ting_get_client()->execute($request);
	return ($object) ? array_shift(ting_add_collection_info(array(ting_add_object_url($object)))) : false;
}	

/**
 * Display a Ting collection of objects.
 *
 * @param string $collection_id
 *    Ting collection ID.
 * @return mixed
 *    Collection object, if found - boolean FALSE if not.
 */
function ting_get_collection_by_id($object_id) {
	$request = ting_get_request_factory()->getCollectionRequest();
	$request->setObjectId($object_id);
	$collection = ting_get_client()->execute($request);
  return ($collection) ? ting_add_collection_info(ting_add_additional_info($collection)) : false;
}

/**
 * Display a Ting collection of objects.
 *
 * @param string $collection_id
 *    Ting collection ID.
 * @return mixed
 *    Collection object, if found - boolean FALSE if not.
 */
function ting_do_search($query, $page = 1, $resultsPerPage = 10, $options = array()) {
	$request = ting_get_request_factory()->getSearchRequest();
	$request->setQuery($query);
	$request->setStart($resultsPerPage * ($page - 1) + 1);
	$request->setNumResults($resultsPerPage);
		
	$request->setFacets((isset($options['facets'])) ? $options['facets'] : array('facet.subject', 'facet.creator', 'dc.type', 'facet.date', 'facet.language'));
	$request->setNumFacets((isset($options['numFacets'])) ? $options['numFacets'] : ((sizeof($request->getFacets()) == 0) ? 0 : 10));
		
	$searchResult = ting_get_client()->execute($request);
		
	//Decorate search result with additional information
	foreach ($searchResult->collections as &$collection)
	{
		$collection = ting_add_collection_info($collection);
		$collection = ting_add_additional_info($collection);
		$collection = ting_sort_objects($collection);
	}
		
	return $searchResult;
}

/**
 * @param string $query The prefix to scan for
 * @param int $numResults The numver of results to return
 * @return TingClientScanResult
 */
function ting_do_scan($query, $numResults = 10) {
	$request = ting_get_request_factory()->getScanRequest();
	$request->setField('phrase.anyIndexes');
	$request->setLower($query);
	$request->setNumResults($numResults);
	return ting_get_client()->execute($request);
}
	
function ting_get_spell_suggestions($word, $numResults = 10) {
	$request = ting_get_request_factory()->getSpellRequest();
	$request->setWord($word);
	$request->setNumResults($numResults);
	return ting_get_client()->execute($request);
}

/**
 * @return TingClient
 */
function ting_get_client() {
	static $client;
	
	if (!isset($client))
	{
		$client = new TingClient(new TingClientDrupal6HttpRequestAdapter(),
																			new TingClientDrupalWatchDogLogger());
	}
	
	return $client;
}

/**
 * @return TingClientRequestFactory
 */
function ting_get_request_factory() {
	static $requestFactory;
	
	if (!isset($requestFactory))
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
		
		$requestFactory = new RestJsonTingClientRequestFactory($urls);
	}
	
	return $requestFactory;
}

function ting_add_collection_info(TingClientObjectCollection $collection)
{
	$collection->url = url('ting/collection/' . $collection->objects[0]->id, array('absolute' => true));
	$types = array();
	foreach ($collection->objects as &$object)
	{
		$object = ting_add_object_url($object);
		$types = array_merge($types, $object->data->type);
	}
	$collection->types = array_unique($types);
	return $collection;
}
	
function ting_sort_objects($collection)
{
	usort($collection->objects, 'ting_compare_objects');
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

function ting_add_object_url(TingClientObject $object)
{
	$object->url = url('ting/object/'.$object->id, array('absolute' => true));
	return $object;
}
	
function ting_add_additional_info($collection)
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
	
function ting_compare_objects($o1, $o2, $criterias = NULL)
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
				return ting_compare_objects($o1, $o2, $criterias);
			}
		}
	}
			
	return 0;
}
