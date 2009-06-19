<?php
// $Id$

/**
 * Admin interface for campaign display rules.
 */
function ding_campaign_admin_rule_form($form_state, $node) {
  $form = array();

  $form['rules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rules'),
    '#attributes' => array('id' => 'campaign-rules'),
    '#tree' => TRUE,
  );

  if (isset($node->campaign_rules) && !empty($node->campaign_rules)) {
    foreach ($node->campaign_rules as $delta => $rule) {
      $form['rules'][$delta] = array(
        '#title' => 'test',
        '#type' => 'ding_campaign_rule',
        '#value' => $rule,
        '#nid' => $node->nid,
      );
    }
  }
  else {
    $form['rules'][0] = array(
      '#title' => 'test',
      '#type' => 'ding_campaign_rule',
      '#nid' => $node->nid,
    );
  }

  $form['new_rule_button'] = array(
    '#type' => 'button',
    '#value' => 'Add another',
    '#ahah' => array(
      'path' => 'node/' . $node->nid . '/campaign_rules/ahah',
      'wrapper' => 'campaign-rules',
      'method' => 'append',
      'effect' => 'slide'
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Validation for campaign display rules form.
 */
function ding_campaign_admin_rule_form_validate($form, &$form_state) {
}

/**
 * Submit handler for campaign display rules form.
 */
function ding_campaign_admin_rule_form_submit($form, &$form_state) {
  // First, remove the existing rules
  db_query("DELETE FROM {ding_campaign_rule} WHERE nid = %d", $node->nid);

  // Then write the new set of rules
  foreach ($form_state['values']['rules'] as $rule) {
    drupal_write_record('ding_campaign_rule', $rule);
  } 
}

/*
  This function is largely based on the poll module, its been simplified for reuse.
  $fields is the specific form elements you want to attach via ahah,
  $name is the form fields array key... e.g. the name for $form['title'] is "title"
*/ 
function ding_campaign_admin_rule_form_ahah($form_state, $node) {
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);

  // Delta is assigned to the count of current elements, so it'll be the 
  // next in the zero-based list.
  $delta = count(element_children($form['rules']));

  $form['rules'][$delta] = array(
    '#type' => 'ding_campaign_rule',
    '#delta' => $delta,
    '#nid' => $node->nid,
  );
  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);
  // Render and return the new field.
  print drupal_to_js(array('data' => drupal_render($form['rules'][$delta]), 'status' => TRUE));
  exit();
}


