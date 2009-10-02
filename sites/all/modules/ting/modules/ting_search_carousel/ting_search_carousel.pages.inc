<?php
// $Id$

/**
 * AHAH callback to retrieve carousel items
 */
function ting_search_carousel_result_ahah($search, $start, $numResults) {
	module_load_include('client.inc', 'ting');	
	
	//retrive all searches to determine which to perform
  $searches = variable_get('ting_search_carousel_searches', array());
	if (!isset($searches[$search]))
	{
		drupal_not_found();
		exit;
	}
	$search = $searches[$search];

	//define a carousel search with valid collections and the highest checked search
	//result index so far
	$carouselSearch = new stdClass();
	$carouselSearch->collections = array();
	$carouselSearch->lastIndex = 0;
	$carouselSearch->more = true;
	
	//retrieve cached search result if it exists
  $cache = cache_get(md5($search['query']), 'cache_ting_search_carousel');
  if ($cache)
  {
  	$carouselSearch = $cache->data;
  }
  
  //if the cached search contains less than the required number of collections
  //the try to retrieve som more
  $resultsPerPage = 10;
  $page = ceil($carouselSearch->lastIndex / $resultsPerPage);
  while ((sizeof($carouselSearch->collections) < ($start+$numResults)) 
				 && $carouselSearch->more)
	{
		$result = ting_do_search($search['query'], ++$page, $resultsPerPage, array('facets' => array()));
    
		if (sizeof($result->collections) > 0)
		{
			foreach ($result->collections as $collection)
			{
				if ($collection->objects[0]->additionalInformation->thumbnailUrl ||
	         	$collection->objects[0]->additionalInformation->detailUrl)
				{
					$carouselSearch->collections[] = $collection;
				}
				$carouselSearch->lastIndex++;
			}
		}
		else
		{
			$carouselSearch->more = false;
		}
	}

	//cache the updated search
	cache_set(md5($search['query']), $carouselSearch, 'cache_ting_search_carousel', CACHE_TEMPORARY);	
	
	//render the collections for the carousel
	$result = '';
	for ($i = $start; $i < ($start + $numResults); $i++)
	{
		if (isset($carouselSearch->collections[$i]))
		{
  		$result .= theme('ting_search_carousel_collection', $carouselSearch->collections[$i]);
		}
	}
	
	echo $result;
	exit;
}