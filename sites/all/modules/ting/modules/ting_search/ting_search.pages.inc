<?php
// $Id$

/**
 * Search result page.
 */
function ting_search_pages_result() {
  $keys = search_get_keys();
  // Loads the JavaScript files.
  ting_search_add_js();

  drupal_add_js(array('tingSearch' => array(
    'result' => array(),
    'keys' => $keys,
    'ting_url' => url('ting/search/js'),
    'content_url' => url('ting/search/content/js'),
    'result_template' => theme('ting_result')
  )), 'setting', 'footer');
  return theme('ting_search_result_page', theme('ding_campaign_relevant_campaigns', (array('search_term' => $keys))));
}

/**
 * Display a Ting collection of objects.
 */
function ting_collection() {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
	
  $facade = new TingClientFacade();
  
	$collection = $facade->getCollection($_REQUEST['object_id']);
	if (!$collection)
	{
		return drupal_not_found();
	}
	
  return theme('ting_collection', $collection);
}

/**
 * Display a single Ting object.
 */
function ting_object($object_id) {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
	
  $facade = new TingClientFacade();
  
	$object = $facade->getObject($object_id);
	if (!$object)
	{
		return drupal_not_found();
	}
	
  return theme('ting_object', $object);
}

/**
 * Return the path to a photo of the object.
 */
function ting_search_cover_path($object_id) {
  return 'covers/' . basename($object_id) . '.jpg';
}

/**
 * Return the URL of a photo of the object.
 */
function ting_search_cover_url(TingClientObject $object, $presetname) {
  $path = ting_search_cover_path($object->id);
  if (file_exists(file_create_path($path))) {
    return imagecache_create_url($presetname, $path);
  }
  elseif ($object->additionalInformation->thumbnailUrl ||
          $object->additionalInformation->detailUrl) {

    // Return an absolute URL like imagecache_create_url() does.
    return url('ting/cover/' . $presetname . '/' . $object->id, array('absolute' => TRUE));
  }
  else {
    return FALSE;
  }
}

/**
 * Return the URL of a photo of the object.
 */
function ting_search_cover($preset, $object_id) {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');

  // $path is without the file_directory_path() prefix.
  $path = ting_search_cover_path($object_id);

  if (!file_exists(file_create_path($path))) {
    $facade = new TingClientFacade();
  	$object = $facade->getObject($object_id);
    if (!$object) {
      drupal_not_found();
      exit;
    }

    if ($object->additionalInformation->thumbnailUrl) {
      $source_url =$object->additionalInformation->thumbnailUrl;
    }
    else if ($object->additionalInformation->detailUrl) {
      $source_url = $object->additionalInformation->detailUrl;
    }
    else {
      drupal_not_found();
      exit;
    }

    $result = drupal_http_request($source_url);
    if ($result->code == 200) {
      file_check_directory(file_create_path(dirname($path)), FILE_CREATE_DIRECTORY);
      file_put_contents($path, $result->data);
      file_save_data($result->data, $path, FILE_EXISTS_REPLACE);
    }
  }

  drupal_goto(imagecache_create_url($preset, $path));
}

/**
 * JSON callback to search Ting.
 */
function ting_search_pages_ting_js() {
  module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
  
	//Get users base query
	$query = (isset($_REQUEST['query'])) ? $_REQUEST['query'] : 'dc.title:danmark';

	//Extend query with selected facets
	if (isset($_GET['facets']) && $_GET['facets']) {
		$facetArray = array();

		$facets = explode(';', $_GET['facets']);
		foreach ($facets as $facet) {
			$facet = explode(':', $facet, 2);
			if ($facet[0]) {
				$facetArray[] = $facet[0].':'.$facet[1];
			}
		}
			
		$query .= ' AND '.implode(' AND ', $facetArray);
	}

	$facade = new TingClientFacade();
	
	$result = $facade->search($query, isset($_REQUEST['page']) ? $_REQUEST['page'] : 1, 10);

  drupal_json($result);
  exit;
}

/**
 * JSON callback to Drupal's content search.
 */
function ting_search_pages_content_js() {
	if (isset($_REQUEST['query'])) {
    $query = trim($_REQUEST['query']);
    $result = array('results' => do_search($query, 'node'));
    $result['count'] = count($result['results']);
    $result['result_html'] = '';
    foreach ($result['results'] as $item) {
      if ($item->type == 'node') {
        $node = node_load($item->sid);
        $result['result_html'] .= node_view($node, TRUE);
      }
    }
    if (!empty($result['result_html'])) {
      $result['result_html'] .= theme('pager', NULL, 10, 0);
    }
	}
  else {
    $result = array(
      'results' => array(),
      'count' => 0,
      'result_html' => '',
    );
  }
  drupal_json($result);
  exit;
}
