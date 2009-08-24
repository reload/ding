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
          case 'borrCardNotFound':
            throw new AlmaClientBorrCardNotFound('Invalid borrower credentials');
            break;
          default:
            throw new AlmaClientCommunicationError('Status is not okay: ' . $message);
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
   * Get reservation branch names from Alma.
   *
   * Formats the list of branches in an array usable for form API selects.
   *
   * @return array
   *    List of branches, keyed by branch_id
   */
  public function get_reservation_branches() {
    // Set a no branch option.
    $branches = array(NULL => '- None -');

    $doc = $this->request('reservation/branches');

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

  /**
   * Get reservation info.
   */
  public function get_reservations($borr_card, $pin_code) {
    $doc = $this->request('patron/reservations', array('borrCard' => $borr_card, 'pinCode' => $pin_code));

    $reservations = array();
    foreach ($doc->getElementsByTagName('reservation') as $item) {
      $reservations[] = array(
        'id' => $item->getAttribute('id'),
        'status' => $item->getAttribute('status'),
        'pickup_branch' => $item->getAttribute('reservationPickUpBranch'),
        'create_date' => $item->getAttribute('createDate'),
        'valid_from' => $item->getAttribute('validFromDate'),
        'valid_to' => $item->getAttribute('validToDate'),
        'queue_no' => $item->getAttribute('queueNo'),
        'organisation_id' => $item->getAttribute('organisationId'),
        'record_id' => $item->getElementsByTagName('catalogueRecord')->item(0)->getAttribute('id'),
        'record_available' => $item->getElementsByTagName('catalogueRecord')->item(0)->getAttribute('isAvailable'),
      );
    }
    usort($reservations, 'AlmaClient::reservation_sort');
    return $reservations;
  }

  /**
   * Helper function for sorting reservations.
   */
  private static function reservation_sort($a, $b) {
    return strcmp($a['create_date'], $b['create_date']);
  }

  /**
   * Get patron's current loans.
   */
  public function get_loans($borr_card, $pin_code) {
    $doc = $this->request('patron/loans', array('borrCard' => $borr_card, 'pinCode' => $pin_code));

    $loans = array();
    foreach ($doc->getElementsByTagName('loan') as $item) {
      $loans[] = array(
        'id' => $item->getAttribute('id'),
        'branch' => $item->getAttribute('loanBranch'),
        'loan_date' => $item->getAttribute('loanDate'),
        'due_date' => $item->getAttribute('loanDueDate'),
        'is_renewable' => ($item->getElementsByTagName('loanIsRenewable')->item(0)->getAttribute('value') == 'yes') ? TRUE : FALSE,
        'record_id' => $item->getElementsByTagName('catalogueRecord')->item(0)->getAttribute('id'),
        'record_available' => $item->getElementsByTagName('catalogueRecord')->item(0)->getAttribute('isAvailable'),
      );
    }
    usort($loans, 'AlmaClient::loan_sort');
    return $loans;
  }

  /**
   * Helper function for sorting reservations.
   */
  private static function loan_sort($a, $b) {
    return strcmp($a['due_date'], $b['due_date']);
  }

  /**
   * Get patron's debts.
   */
  public function get_debts($borr_card, $pin_code) {
    $doc = $this->request('patron/debts', array('borrCard' => $borr_card, 'pinCode' => $pin_code));

    $data = array(
      'total_formatted' => $doc->getElementsByTagName('debts')->item(0)->getAttribute('totalDebtAmountFormatted'),
      'debts' => array(),
    );

    foreach ($doc->getElementsByTagName('debt') as $item) {
      $data['debts'][] = array(
        'id' => $item->getAttribute('debtId'),
        'date' => $item->getAttribute('debtDate'),
        'type' => $item->getAttribute('debtType'),
        'amount' => $item->getAttribute('debtAmount'),
        'amount_formatted' => $item->getAttribute('debtAmountFormatted'),
        'note' => $item->getAttribute('debtNote'),
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

