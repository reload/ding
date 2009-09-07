<?php
function template_preprocess_ting_collection(&$variables) {
	// preprocess the collection object here!

	// group by type and language
	$collection = $variables["collection"];
	
	$sorted = array();
	foreach ($collection as $value) {
		foreach ($value as $object) {
			$type_key = $object->data->type['0'] . " (" . $object->data->language['1'] . ")";
			
			if(trim($type_key) != "()")
				$sorted[$type_key][] = $object;
		}
	}
	
	ksort($sorted); // ... not always good - arabic comes before danish...
	
	$variables["sorted_collection"] = $sorted;
	
} 