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
    $items[] = theme('alma_user_list_item', $rec);
  }

  return theme('item_list', $items, NULL, 'ul', array('class' => 'record-list'));
}

/**
 * Theming of reservation details form.
 */
function theme_alma_user_forms_reservation_details($form) {
  $output = '';
  $branches = alma_client_get_branches();
  module_load_include('client.inc', 'ting');

  if (!empty($form['reservation_data']['#grouped']['fetchable'])) {
    $header = array(
      t('Title'),
      t('Pickup number'),
      t('Expiry date'),
      t('Pickup branch'),
    );

    $rows = array();

    foreach ($form['reservation_data']['#grouped']['fetchable'] as $item) {
      $rows[] = array(
        theme('alma_user_list_item', $item),
        $item['pickup_number'],
        $item['pickup_expire_date'],
        $branches[$item['pickup_branch']],
      );
    }

    $output .= theme('table', $header, $rows, array('id' => 'reservations-fetchable'), t('Reservations ready for pickup'));
  }

  if (!empty($form['reservation_data']['#grouped']['active'])) {
    $header = array(
      t('Select'),
      t('Reservation number'),
      t('Title'),
      t('Reserved'),
      t('Valid to'),
      t('Pickup branch'),
      t('Queue number'),
    );
    $rows = array();

    foreach ($form['reservation_data']['#grouped']['active'] as $item) {
      $rows[] = array(
        drupal_render($form['reservations'][$item['id']]),
        $item['id'],
        theme('alma_user_list_item', $item),
        $item['create_date'],
        $item['valid_to'],
        $branches[$item['pickup_branch']],
        $item['queue_no'],
      );
    }

    $output .= theme('table', $header, $rows, array('id' => 'reservations-active'), t('Active reservations'));
  }

  if (empty($output)) {
    return t('No reservations found.');
  }

  $output .= drupal_render($form);
  return $output;
}

/**
 * Theming of loan details form.
 */
function theme_alma_user_forms_loan_details($form) {
  module_load_include('client.inc', 'ting');
  $header = array(t('Select'), t('Title'), t('Loan date'));
  $groups = array();
  $output = '';

  foreach ($form['loan_data']['#grouped'] as $date => $group) {
    if ($date == 'overdue') {
      $table_title = t('Overdue loans');
    }
    else {
      $table_title = t('Due in @count days, @date', array('@date' => date('d/m/y', strtotime($date)), '@count' => ceil((strtotime($date) - $_SERVER['REQUEST_TIME']) / 86400)));
    }
    $rows = array();

    foreach ($group as $loan) {
      $rows[] = array(
        drupal_render($form['loans'][$loan['id']]),
        theme('alma_user_list_item', $loan),
        $loan['loan_date'],
      );
    }
    if (!empty($rows)) {
      $output .= drupal_render($form['renew_all']);
      $output .= theme('table', $header, $rows, array(), $table_title);
    }
  }
  if (empty($output)) {
    return t('No loans found.');
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

/**
 * Render label for reservation or loan in a list.
 */
function theme_alma_user_list_item($item) {
  // First try looking up the title and book data from Ting.
  if (intval($item['id']) > 0) {
    $ting_object = ting_get_object_by_local_id($item['record_id']);
    if ($ting_object) {
      return theme('ting_object_title', $ting_object, TRUE);
    }
  }

  // If Ting data is not available, get it from Alma instead.
  $data = alma_client_catalogue_record_detail($item['record_id']);
  $details = $data['records'][$item['record_id']];

  $output = '<span class="title">' . $details['titles'][0] . '</span> ';

  if (!empty($details['authors'])) {
    $output .= '<span class="byline">' . t('by') . '</span> ';
    $output .= implode(', ', $details['authors']);
  }

  // Remote loans have negative IDs (undocumented Axiell "feature").
  if (intval($item['id']) < 0) {
    $output .= ' <span class="remote">(' . t('remote loan') . ')</span>';
  }

  return $output;
}

