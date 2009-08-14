<?php

class OpenSpellSuggestion
{
	
	public $word;
	public $weight;
	
	public function __construct($word, $weight)
	{
		$this->word = $word;
		$this->weight = $weight;
	}

}
