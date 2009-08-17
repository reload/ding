<?php
// $Id$

/**
 * JSON callback to Ting search autocomplete
 */
function ting_search_autocomplete_js($string) {
	$items = cache_get(md5($string), 'cache_ting_search_autocomplete');
	if (!$items)
	{
		module_load_include('php', 'ting', 'lib/openspell-client/OpenSpellService');
		module_load_include('php', 'ting', 'lib/facade/TingClientFacade');
		
		$terms = array();
		if ($string && (strlen($string) > 0))
		{
			$scanResults = TingClientFacade::scan($string, 10);
			foreach ($scanResults->terms as $term)
			{
				$term->score = $term->count;
				$terms[strtolower($term->name)] = $term;
			}
			
			//TODO: Consider moving base url to admin settings
			$openSpell = new OpenSpellService('http://didicas.dbc.dk/openspell/server.php', 'json');
			$suggestions = $openSpell->getSuggestions($string);
			foreach ($suggestions as $suggestion)
			{
				if (!isset($terms[strtolower($suggestion->word)]))
				{
					$suggestScan = TingClientFacade::scan($suggestion->word, 1);
					if (sizeof($suggestScan->terms) > 0)
					{
						$term = array_shift($suggestScan->terms);
						$term->score = $term->count * $suggestion->weight;
						$terms[$term->name] = $term;
					}
				}
			}
		}
		
		uasort($terms, 'ting_search_autocomplete_term_sort');
		$terms = array_reverse($terms);
	
		$items = array();
		foreach ($terms as $term)
		{
			$items[$term->name] = t('!ord (!resultater)', 
															array('!ord' => $term->name,
																		'!resultater' => format_plural($term->count, '1 resultat', '@count resultater')));
		}
		
		$items = array_slice($items, 0, 10);
		cache_set(md5($string), $items, 'cache_ting_search_autocomplete', CACHE_TEMPORARY);
	}
	drupal_json($items);
}

function ting_search_autocomplete_term_sort($t1, $t2) {
	if ($t1->score > $t2->score){
		return 1;
	} elseif ($t2 > $t1->score) {
		return -1;
	} else {
		return 0;
	}
}
