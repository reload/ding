<?php
// $Id$

/**
 * Return the URL for the cover of an object
 */
function ting_covers_object($preset, $object_id) {
	module_load_include('client.inc', 'ting');

  // $path is without the file_directory_path() prefix.
  $path = ting_covers_object_path($object_id);

  if (!file_exists(file_create_path($path))) {
    $object = ting_get_object_by_id($object_id, true);
    
    if (!$object) {
      ting_covers_goto_default($preset);
    }
  
    if ($object->additionalInformation->detailUrl) {
      $source_url = $object->additionalInformation->detailUrl;
    }
    elseif ($object->additionalInformation->thumbnailUrl) {
      $source_url =$object->additionalInformation->thumbnailUrl;
    }
    else {
      ting_covers_goto_default($preset);
    }
    
    $result = drupal_http_request($source_url);
    if ($result->code == 200) {
      file_check_directory(file_create_path(dirname($path)), FILE_CREATE_DIRECTORY);
      file_save_data($result->data, $path, FILE_EXISTS_REPLACE);
    }
  }

  drupal_goto(imagecache_create_url($preset, $path));
}

/**
 * Return the URL for the cover for a collection
 */
function ting_covers_collection($preset, $object_id) {
	module_load_include('client.inc', 'ting');

  //$path is without the file_directory_path() prefix.
  $path = ting_covers_collection_path($object_id);

  if (!file_exists(file_create_path($path))) {
    $collection = ting_get_collection_by_id($object_id, true);
    if (!$collection) {
      ting_covers_goto_default($preset);
    }
    
    //go through the objects in the collection looking for the first object with a cover
    $source_url = false;
    foreach ($collection->objects as $object) {
	    if ($object->additionalInformation->detailUrl) {
	      $source_url = $object->additionalInformation->detailUrl;
	      break; //we have a full high resolution image so move along!
	    }
	    elseif ($object->additionalInformation->thumbnailUrl) {
	      $source_url =$object->additionalInformation->thumbnailUrl;
	    }
    }
        
    //no cover found
    if (!$source_url) {
      ting_covers_goto_default($preset);
    }

    $result = drupal_http_request($source_url);
    if ($result->code == 200) {
      file_check_directory(file_create_path(dirname($path)), FILE_CREATE_DIRECTORY);
      file_save_data($result->data, $path, FILE_EXISTS_REPLACE);
    }
  }

  drupal_goto(imagecache_create_url($preset, $path));
}
