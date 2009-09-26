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
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');

  $facade = new TingClientFacade();

  $object = $facade->getObject($object_id);

  if (!$object) {
    return FALSE;
  }
  return $object;
}

/**
 * Display a Ting collection of objects.
 *
 * @param string $collection_id
 *    Ting collection ID.
 * @return mixed
 *    Collection object, if found - boolean FALSE if not.
 */
function ting_get_collection_by_id($collection_id) {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');

  $facade = new TingClientFacade();

  $collection = $facade->getCollection($collection_id);
  if (!$collection) {
    return FALSE;
  }

  return $collection;
}

