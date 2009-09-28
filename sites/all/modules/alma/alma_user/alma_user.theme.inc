<?php
// $Id$

/**
 * Theming of a records list.
 *
 * Usually a list of reservations, books, etc, coming from Alma.
 */
function theme_alma_user_record_list($records) {
  $items = array();

  foreach ($records as $rec) {
    // TODO: We need to get the name from Ting based on the record ID.
    $items[] = $rec['record_id'];
  }

  return theme('item_list', $items, NULL, 'ul', array('class' => 'record-list'));
}

/**
 * Theming of reservation details form.
 */
function theme_alma_user_forms_reservation_details($form) {
  $branches = alma_client_get_branches();
  module_load_include('client.inc', 'ting');

  $rows = array();
  foreach ($form['reservation_data']['#value'] as $key => $data) {
    $ting_object = ting_get_object_by_local_id($data['record_id']);
    $rows[] = array(
      drupal_render($form['reservations'][$data['id']]),
      theme('ting_object_title', $ting_object, TRUE),
      $data['valid_to'],
      $branches[$data['pickup_branch']],
      $data['queue_no'],
    );
  }
  $header = array(
    t('Select'),
    t('Title'),
    t('Valid to'),
    t('Pickup branch'),
    t('Queue number'),
  );

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theming of loan details form.
 */
function theme_alma_user_forms_loan_details($form) {
  module_load_include('client.inc', 'ting');
  $header = array(t('Select'), t('Title'));
  $groups = array();
  $output = '';

  foreach ($form['loan_data']['#grouped'] as $date => $group) {
    if ($date == 'overdue') {
      $table_title = t('Overdue loans');
    }
    else {
      $table_title = t('Due in @count days, @date', array('@date' => $date, '@count' => ceil((strtotime($date) - $_SERVER['REQUEST_TIME']) / 86400)));
    }
    $rows = array();

    foreach ($group as $loan) {
      $ting_object = ting_get_object_by_local_id($loan['record_id']);

      $rows[] = array(
        drupal_render($form['loans'][$loan['id']]),
        theme('ting_object_title', $ting_object, TRUE),
      );
    }

    $output .= theme('table', $header, $rows, array(), $table_title);
  }
  $output .= drupal_render($form);
  return $output;
}

