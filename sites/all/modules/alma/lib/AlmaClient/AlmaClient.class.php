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
    $url = $this->base_url . $method;

    if (!empty($params)) {
      $sparams = array();
      foreach ($params as $key => $value) {
        $sparams[] = rawurlencode($key) . '='. rawurlencode($value);
      }

      $url .= '?' . implode('&', $sparams);
    }

    $data = qp($url);

    if ($data->find('status')->attr('value') == 'ok') {
      return $data;
    }
    else {
      // TODO: Make more descriptive exceptions.
      throw new Exception('Status is not okay');
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

    $data = $this->request('organisation/branches');

    foreach ($data->find('branch')->matches() as $branch) {
      $branches[$branch->attr('id')] = $branch->find('name')->text();
    }

    return $branches;
  }

  /**
   * Get patron information from Alma
   */
  public function get_patron_info($borr_card, $pin_code) {
    $data = $this->request('patron/information', array('borrCard' => $borr_card, 'pinCode' => $pin_code));
    return $data;
  }
}

