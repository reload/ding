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
 * @param bool $enrich
 *    Whether to enrich the object with additional information, covers etc.
 * @return mixed
 *    Object, if found - boolean FALSE if not.
 */
function ting_get_object_by_id($object_id, $enrich = FALSE) {
  static $cache = array();

  if (isset($cache[$object_id])) {
    $object = $cache[$object_id];
  }
  else {
    $request = ting_get_request_factory()->getObjectRequest();
    $request->setObjectId($object_id);
    $request = ting_add_agency($request);
    $object = ting_execute($request);

    if ($object) {
      $object = ting_add_object_info($object);
    }
  }

  if ($object && $enrich && empty($object->enriched)) {
    $object = array_shift(ting_add_additional_info(array($object)));
    $object->enriched = TRUE;
  }

  $cache[$object_id] = $object;

  return $object;
}

/**
 * Load an object from Ting by local ID (faust number).
 *
 * @param string $local_id
 *    Local identifier
 * @param bool $enrich
 *    Whether to enrich the object with additional information, covers etc.
 * @return TingClientObject
 */
function ting_get_object_by_local_id($local_id, $enrich = false) {
	$request = ting_get_request_factory()->getObjectRequest();
	$request->setLocalId($local_id);
	$request = ting_add_agency($request);
	$object = ting_execute($request);

	if ($object) {
		$object = ting_add_object_info($object);
		$object = ($enrich) ? array_shift(ting_add_additional_info(array($object))) : $object;
	}
	return ($object) ? $object : false;
}

/**
 * Display a Ting collection of objects.
 *
 * @param string $collection_id
 *    Ting collection ID.
 * @param bool $enrich
 *    Whether to enrich objects in the collection with additional information, covers etc.
 * @return mixed
 *    Collection object, if found - boolean FALSE if not.
 */
function ting_get_collection_by_id($object_id, $enrich = FALSE) {
  static $cache = array();

  if (isset($cache[$object_id])) {
    $collection = $cache[$object_id];
  }
  else {
    $request = ting_get_request_factory()->getCollectionRequest();
    $request->setObjectId($object_id);
    $request = ting_add_agency($request);
    $collection = ting_execute($request);

    if ($collection) {
      $collection = ting_add_collection_info($collection);
      $collection = ting_sort_objects($collection);
    }
  }

  if ($collection && $enrich && empty($collection->enriched)) {
    $collection = ting_add_additional_info($collection);
    $collection->enriched = TRUE;
  }

  $cache[$object_id] = $collection;

  return $collection;
}

/**
 * Return the possible sort keys and their corresponding labels.
 *
 * @return
 *   A key/value array of sort keys and labels.
 */
function ting_search_sort_options() {
  return array(
    '' => t('Relevance'),
    'title_ascending' => t('Title – A → Z'),
    'title_descending' => t('Title – Z → A'),
    'creator_ascending' => t('Author – A → Z'),
    'creator_descending' => t('Author – Z → A'),
    'date_descending' => t('Year of publication – newest first'),
    'date_ascending' => t('Year of publication – oldest first'),
  );
}

/**
 * Performs a search agains the 
 *
 * @param string $query
 *    The search query
 * @param int $page
 *    The page number to retrieve search results for
 * @param int $resultsPerPage
 *    The number of results to include per page
 * @param array $options
 *    Options to pass to the search. Possible options are:
 *    - facets: Array of facet names for which to return results. Default: facet.subject, facet.creator, facet.type, facet.date, facet.language
 *    - numFacets: The number of terms to include with each facet. Default: 10
 *    - enrich: Whether to include additional information and cover images with each object. Default: false
 *    - sort: The key to sort the results by. Default: "" (corresponds to relevance). The possible values are defined by the sortType type in the XSD.
 *    - supportingTypes: Whether to include supporting types such as reviews. Default: false
 * @return TingClientSearchResult
 *    The search result
 */
function ting_do_search($query, $page = 1, $resultsPerPage = 10, $options = array()) {
	$request = ting_get_request_factory()->getSearchRequest();
	$request->setQuery($query);
	$request = ting_add_agency($request);
	$request->setStart($resultsPerPage * ($page - 1) + 1);
	$request->setNumResults($resultsPerPage);
  
	$request = (!isset($options['supportingTypes']) || !$options['supportingTypes']) ? ting_remove_supporting_types($request) : $request;
	$request->setFacets((isset($options['facets'])) ? $options['facets'] : array('facet.subject', 'facet.creator', 'facet.type', 'facet.date', 'facet.language'));
	$request->setNumFacets((isset($options['numFacets'])) ? $options['numFacets'] : ((sizeof($request->getFacets()) == 0) ? 0 : 10));
  $request->setSort((isset($options['sort'])) ? $options['sort'] : '');
  $request->setAllObjects(isset($options['allObjects']) ? $options['allObjects'] : FALSE);

	$searchResult = ting_execute($request);

	//Decorate search result with additional information
	foreach ($searchResult->collections as &$collection) {
		$collection = ting_add_collection_info($collection);
		if (isset($options['enrich']) && $options['enrich']) {
			$collection = ting_add_additional_info($collection);
		}
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
	return ting_execute($request);
}

/**
 * @param string $word The word to get spell suggestions for
 * @param $numResults The number of results to return
 * @return array An array of TingClientSpellSuggestion objects
 */
function ting_get_spell_suggestions($word, $numResults = 10) {
	$request = ting_get_request_factory()->getSpellRequest();
	$request->setWord($word);
	$request->setNumResults($numResults);
	return ting_execute($request);
}

/**
 * @param string $isbn ISBN number to get recommendations from
 * @param $numResults The number of results to return
 * @return array An array of TingClientObjectRecommendation objects
 */
function ting_get_object_recommendations($isbn, $numResults = 10) {
	$request = ting_get_request_factory()->getObjectRecommendationRequest();
	$request->setIsbn($isbn);
	$request->setNumResults($numResults);
	return ting_execute($request);
}

/**
 * Perform a request against Ting and perform error handling if necessary
 * 
 * @param $request The request
 * @return mixed Result of the request or false if an error occurs 
 */
function ting_execute($request)
{
	try {
  	return ting_get_client()->execute($request);
	} catch (TingClientException $e) {
	  watchdog('ting client', 'Error performing request: '.$e->getMessage(), var_export($request, true), WATCHDOG_ERROR, 'http://'.$_SERVER["HTTP_HOST"].$_SERVER["REQUEST_URI"]);
		return false;
	}
}

/**
 * Retrieves an initialized Ting client with appropriate request adapter and logger 
 * 
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
 * Retrieves an initialized Ting client request factory.
 * 
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
		$object = ting_add_object_info($object);
		if (isset($object->data->type) && is_array($object->data->type))
		{
  		$types = array_merge($types, $object->data->type);
		}
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

function ting_add_object_info(TingClientObject $object) {
	$object->url = url('ting/object/'.$object->id, array('absolute' => true));

  if ($object->data->type[0] == 'Netdokument') {
    foreach ($object->data->identifier as $identifier) {
      if ($identifier->type == TingClientObjectIdentifier::URL) {
        $object->online_url = ting_rewrite_download_url($identifier->id);
      }
    }
  }

	return $object;
}

function ting_add_additional_info($collection) {
	//Add additional information info for cover images
	$faust_numbers = array();

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
	extract($addiVariables);
	$additionalInformationService = new AdditionalInformationService($wsdlUrl, $username, $group, $password);

	$objects = (isset($collection->objects)) ? $collection->objects : $collection;

	foreach($objects as $object)
	{
		if (isset($object->data->identifier))
		{
			foreach ($object->data->identifier as $identifier)
			{
				if ($identifier->type == TingClientObjectIdentifier::FAUST_NUMBER)
				{
					$id_parts = explode('|', $identifier->id, 2);
					$faust_numbers[] = $id_parts[0];
				}
			}
		}
	}
	
	if (sizeof($faust_numbers) > 0)
	{
		try
		{
			$additionalInformations = $additionalInformationService->getByFaustNumber($faust_numbers);
			foreach ($additionalInformations as $id => $ai)
			{
				foreach ($objects as &$object)
				{
					if (isset($object->data->identifier))
					{
						foreach ($object->data->identifier as $identifier)
						{
							if (($identifier->type == TingClientObjectIdentifier::FAUST_NUMBER) &&
							    (strpos($id, $identifier->id) == 0))
							{
								$object->additionalInformation = $ai;
							}
						}
					}
				}
			}
		}
		catch (AdditionalInformationServiceException $e)
		{
			watchdog('TingClient', 'Error retrieving additional information and covers: '.$e->getMessage(), $isbns, WATCHDOG_ERROR);
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
			$o1Check = (isset($o1->data->$attribute) && is_array($o1->data->$attribute)) ? in_array($value, $o1->data->$attribute) : false;
			$o2Check = (isset($o2->data->$attribute) && is_array($o2->data->$attribute)) ? in_array($value, $o2->data->$attribute) : false;
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

function ting_add_agency(TingClientAgentRequest $request)
{
	if ($agency = variable_get('ting_agency', false))
	{
		$request->setAgency($agency);
	}
	return $request;
}

function ting_remove_supporting_types(TingClientSearchRequest $request)
{
	$supportingTypes = array('materialevurdering*', 'anmeldelse');
	foreach ($supportingTypes as &$type)
	{
		$type = 'facet.type='.$type;
	}
	
	$request->setQuery($request->getQuery().' NOT ('.implode(' OR ', $supportingTypes).')');
	return $request;
}

/**
 * Rewrite the download URL of external restricted-access ressources.
 *
 * This allow access to the ressources through the library's proxy server.
 *
 * Only URLs refererring to sites specified on the Ting settings page are
 * modified; other URLs are returned unmodified.
 */
function ting_rewrite_download_url($url) {
  $host = parse_url($url, PHP_URL_HOST);
  if (in_array($host, variable_get('restricted_access_proxy_hostnames', array()))) {
    $prefix = variable_get('restricted_access_proxy_prefix', '');
    if ($prefix) {
      $url = $prefix . $url;
    }
  }
  return $url;
}

