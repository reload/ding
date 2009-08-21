<?php
// $Id$

/**
 * JSON callback to Ting reference collection autocomplete
 */
function ting_reference_collection_autocomplete_js() {
	module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
	
	$string = $_REQUEST['query'];
	
	$items = array();
	if ($string && (strlen($string) > 0))
	{
		$searchResult = TingClientFacade::search($string, 1, 10, array('facets' => array()));
		foreach ($searchResult->collections as $collection)
		{
			$object = array_shift($collection->objects);
			$items[$object->id] = t('!author: !title (!manifestations)', 
															array('!author' => $object->data->creator[0],
																		'!title' => $object->data->title[0],
																		'!manifestations' => format_plural(sizeof($collection->objects), '1 manifestation', '@count manifestations')));
		}
	}
	
	echo ting_reference_autocomplete_format_items($items);
	exit;
}

/**
 * JSON callback to Ting reference collection autocomplete
 */
function ting_reference_object_autocomplete_js() {
	module_load_include('php', 'ting', 'lib/facade/TingClientFacade');

	$string = $_REQUEST['query'];
	
	$items = array();
	if ($string && (strlen($string) > 0))
	{
		$searchResult = TingClientFacade::search($string, 1, 10, array('useCollections' => 'false', 'facets' => array()));
		
		foreach ($searchResult->collections as $collection)
		{
			$object = array_shift($collection->objects);
			$items[$object->id] = t('!author: !title (!type, !year, !id)', 
															array('!author' => $object->data->creator[0],
																		'!title' => $object->data->title[0],
																		'!type' => $object->data->type[0],
																		'!year' => $object->data->date[0],
																		'!id' => $object->data->identifier[0]));
		}
	}

	
	echo ting_reference_autocomplete_format_items($items);
	exit;
}

function ting_reference_autocomplete_format_items($items)
{
	$return = array();
	foreach ($items as $id => $value)
	{
		$return[] = $value.'|'.$id;		
	}
	return implode("\n", $return);
}
