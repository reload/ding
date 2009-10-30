<?php
// $Id$

/**
 * Theming of a records list.
 *
 * Usually a list of reservations, books, etc, coming from Alma.
 */
function theme_alma_user_record_list($records) {
  module_load_include('client.inc', 'ting');
  $items = array();

  foreach ($records as $rec) {
    // Load the object from Ting to get the title.
    $object = ting_get_object_by_local_id($rec['record_id']);
    $items[] = theme('ting_object_title', $object);
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

/**
 * Theming of debt details form.
 */
function theme_alma_user_forms_debts_details($form) {
  $header = array(
    t('Select'),
    t('Note'),
    t('Date'),
    t('Amount'),
  );

  $rows = array();
  foreach ($form['debt_data']['#value']['debts'] as $key => $data) {
    $rows[$data['id']] = array(
      drupal_render($form['debts'][$data['id']]),
      $data['note'],
      $data['date'],
      $data['amount_formatted'],
    );
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

