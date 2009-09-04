<?php
function template_preprocess_ting_collection(&$variables) {
	// preprocess the collection object here!
	
	//$variables["pladder"] = "hej";
	//dpm($variables);
	
	// group by type and language
	$collection = $variables["collection"];
//	$variables["materialtypes"] = get_material_types($collection);
	
	
	

	
	$sorted = array();
	foreach ($collection as $value) {
		foreach ($value as $object) {
			$type_key = $object->data->type['0'] . " (" . $object->data->language['1'] . ")";
			
			if(trim($type_key) != "()")
				$sorted[$type_key][] = $object;
		}
	}
	
	$variables["sorted_collection"] = $sorted;

	//dpm($sorted);
	
	
} 


function get_material_types($collection) {
	$materialtypes = array();
	foreach ($collection->objects as $key => $value) {
		$type = $collection->objects[$key]->data->type['0'];
		$lang = $collection->objects[$key]->data->language['1'];
		$type_key = $type .' ('.$lang.')';
		
		if (!in_array($type_key, $materialtypes)) {
			$materialtypes[] = $type_key;
		}
	}
	
	return $materialtypes;
}
