<?php

require_once(dirname(__FILE__).'/AdditionalInformationServiceException.php');
require_once(dirname(__FILE__).'/AdditionalInformation.php');

class AdditionalInformationService {

	private $wsdlUrl;
	private $username;
	private $group;
	private $password;

	public function __construct($wsdlUrl, $username, $group, $password)
	{
		$this->wsdlUrl = $wsdlUrl;
		$this->username = $username;
		$this->group = $group;
		$this->password = $password;
	}
	
	public function getByIsbn($isbn)
	{
		$isbn = str_replace('-', '', $isbn);
		
		$authInfo = array('authenticationUser' => $this->username,
											'authenticationGroup' => $this->group,
											'authenticationPassword' => $this->password);
		
		$isbnIdentifiers = array();
		if (!is_array($isbn))
		{
			$isbn = array($isbn);
		}
		foreach ($isbn as $i)
		{
			$isbnIdentifiers[] = array('isbn' => $i);
		}
		
		$client = new SoapClient($this->wsdlUrl);
		
		$startTime = explode(' ', microtime());	
		$response = $client->additionalInformation(array(
													'authentication' => $authInfo,
													'identifier' => $isbnIdentifiers));
		$stopTime = explode(' ', microtime());
    $time = floatval(($stopTime[1]+$stopTime[0]) - ($startTime[1]+$startTime[0]));
		watchdog('addi', 'Completed request ('.round($time, 3).'s)', $isbnIdentifiers, WATCHDOG_INFO, 'http://'.$_SERVER["HTTP_HOST"].$_SERVER["REQUEST_URI"]);
    
		if ($response->requestStatus->statusEnum != 'ok')
		{
			throw new AdditionalInformationServiceException($response->requestStatus->statusEnum.': '.$response->requestStatus->errorText);
		}
		
		if (!is_array($response->identifierInformation))
		{
			$response->identifierInformation = array($response->identifierInformation);	
		}
		
		$additionalInformations = array();
		foreach($response->identifierInformation as $info)
		{
			$thumbnailUrl = $detailUrl = NULL;
			if (isset($info->identifierKnown) &&
					$info->identifierKnown)
			{
				if (!is_array($info->image))
				{
					$info->image = array($info->image);
				}
				
				foreach ($info->image as $image)
				{
					switch ($image->imageSize) {
						case 'thumbnail':
							$thumbnailUrl = $image->_; 
							break;
						case 'detail':
							$detailUrl = $image->_; 
							break;
						default:
							throw new AdditionalInformationServiceException('Unrecognized image size '.$image->imageSize);
					}
				}			
	
				$additionalInfo = new AdditionalInformation($thumbnailUrl, $detailUrl);
				$additionalInformations[$info->identifier->isbn] = $additionalInfo;
			}
		}
		return $additionalInformations;		
	}

}