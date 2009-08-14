<?php
// $Id$

/**
 * JSON callback to Ting search autocomplete
 */
function ting_search_autocomplete_js($string) {
	module_load_include('php', 'ting', 'lib/openspell-client/OpenSpellService');
	
	$words = array();
	if ($string)
	{
		$openSpell = new OpenSpellService('http://didicas.dbc.dk/openspell/server.php', 'json');
		$suggestions = $openSpell->getSuggestions($string);
		foreach ($suggestions as $suggestion)
		{
			$words[$suggestion->word] = $suggestion->word;
		}
	}
	
	drupal_json($words);
}
