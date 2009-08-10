<?php
// $Id$

/**
 * @file AlmaClient.php
 * Provides a client for the Axiell Alma library information webservice.
 */
class AlmaClient {
  /**
   * @var AlmaClientBaseURL
   * The base server URL to run the requests against.
   */
  private $base_url;

  /**
   * Constructor, checking if we have a sensible value for $base_url.
   */
  function __construct($base_url) {
    if (/*stripos('http', $base_url) === 0 &&*/ filter_var($base_url, FILTER_VALIDATE_URL, FILTER_FLAG_SCHEME_REQUIRED)) {
      $this->base_url = $base_url;
    }
    else {
      // TODO: Use a specialised exception for this.
      throw new Exception('Invalid base URL: ' . $base_url);
    }
  }

  /**
   * Perform request to the Alma server
   *
   * @param string $method
   *    The REST method to call e.g. 'patron/status'. borrCard and pinCode
   *    are required for all request related to library patrons.
   * @param array $params
   *    Query string parameters in the form of key => value.
   * @return object
   *    A SimpleXML object with the response.
   */
  public function request($method, $params = array()) {
    // For use with a non-Drupal-system, we should have a way to swap
    // the HTTP client out.
    $request = drupal_http_request(url($this->base_url . $method, array('query' => $params)));

    if ($request->code == 200) {
      // Since we currently have no neat for the more advanced stuff
      // SimpleXML provides, we'll just use DOM, since that is a lot
      // faster in most cases.
      $doc = new DOMDocument();
      $doc->loadXML($request->data);
      if ($doc->getElementsByTagName('status')->item(0)->getAttribute('value') == 'ok') {
        return $doc;
      }
      else {
        $message = $doc->getElementsByTagName('status')->item(0)->getAttribute('key');
        switch($message) {
          case '':
            throw new AlmaClientBorrCardNotFound('Invalid borrower credentials');
            break;
          default:
            throw new AlmaClientCommunicationError('Status is not okay');
        }
      }
    }
    else {
      throw new AlmaClientHTTPError('Request error: ' . $request->code . $request->error);
    }
  }

  /**
   * Get branch names from Alma.
   *
   * Formats the list of branches in an array usable for form API selects.
   *
   * @return array
   *    List of branches, keyed by branch_id
   */
  public function get_branches() {
    // Set a no branch option.
    $branches = array(NULL => '- None -');

    $doc = $this->request('organisation/branches');

    foreach ($doc->getElementsByTagName('branch') as $branch) {
      $branches[$branch->getAttribute('id')] = $branch->getElementsByTagName('name')->item(0)->nodeValue;
    }

    return $branches;
  }

  /**
   * Get patron information from Alma
   */
  public function get_patron_info($borr_card, $pin_code) {
    $doc = $this->request('patron/information', array('borrCard' => $borr_card, 'pinCode' => $pin_code));

    $info = $doc->getElementsByTagName('patronInformation')->item(0);

    $data = array(
      'patron_id' => $info->getAttribute('patronId'),
      'patron_name' => $info->getAttribute('patronName'),
      'addresses' => array(),
      'mails' => array(),
      'phones' => array(),
    );

    foreach ($info->getElementsByTagName('address') as $address) {
      $data['addresses'][] = array(
        'id' => $address->getAttribute('id'),
        'type' => $address->getAttribute('type'),
        'active' => (bool) ($address->getAttribute('isActive') == 'yes'),
        'care_of' => $address->getAttribute('careOf'),
        'street' => $address->getAttribute('streetAddress'),
        'postal_code' => $address->getAttribute('zipCode'),
        'city' => $address->getAttribute('city'),
        'country' => $address->getAttribute('country'),
      );
    }

    foreach ($info->getElementsByTagName('emailAddress') as $mail) {
      $data['mails'][] = array(
        'id' => $mail->getAttribute('id'),
        'mail' => $mail->getAttribute('address'),
      );
    }

    foreach ($info->getElementsByTagName('phoneNumber') as $phone) {
      $data['phones'][] = array(
        'id' => $phone->getAttribute('id'),
        'phone' => $phone->getAttribute('localCode'),
        'sms' => (bool) ($phone->getElementsByTagName('sms')->item(0)->getAttribute('useForSms') == 'yes'),
      );
    }
    return $data;
  }
}

/**
 * Define exceptions for different error conditions inside the Alma client.
 */

class AlmaClientInvalidURLError extends Exception {
}


class AlmaClientHTTPError extends Exception {
}


class AlmaClientCommunicationError extends Exception {
}


class AlmaClientBorrCardNotFound extends Exception {
}

