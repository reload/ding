<?php

require_once(dirname(__FILE__).'/OpenSpellServiceException.php');
require_once(dirname(__FILE__).'/OpenSpellSuggestion.php');

class OpenSpellService
{
	private $baseUrl;
	private $format;
	
	public function __construct($baseUrl, $format)
	{
		$this->baseUrl = $baseUrl;
		$this->format = $format;
	}
	
	public function getSuggestions($word, $numResults = 10)
	{
		$args = array('word='.$word, 'number='.$numResults, 'outputType='.$this->format);
		$url = $this->baseUrl.'?'.implode('&', $args);
		
		$response = drupal_http_request($url);
		
		$response = json_decode($response->data);
		if (isset($response->error))
		{
			throw new OpenSpellException($response->error.' ('.$url.')');
		}
		
		$suggestions = array();
		foreach ($response->term as $term)
		{
			$suggestions[] = new OpenSpellSuggestion($term->suggestion, floatval(str_replace(',', '.', $term->weight)));
		}
		return $suggestions;
	}
	
}

?>