<?php
// $Id$

/**
 * @file similarterms.inc
 * A Panels content type providing similar content based on Taxonomy 
 * terms via the similarterms module.
 */

/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function similarterms_similarterms_ctools_content_types() {
  return array(
    // This content type relates to nodes, so we'll stick it in there.
    'title' => t('Node'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function similarterms_similarterms_content_type_content_types() {
  return array(
    'similarterms' => array(
      'title' => t('Similar by terms'),
      'description' => t('Similar content by taxonomy terms.'),
      'category' => t('Node'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
    ),
  );
}

/**
 * Return the pane contents.
 */
function similarterms_similarterms_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  if ($node) {
    $block->title = t('Similar content');
    $list = similarterms_list($conf['vocabulary'], $node->nid);
    $block->content = theme('similarterms', $conf['display_options'], $list);
  }
  else {
    $block->content = '';
  }
  return $block;
}

/**
 * Return the pane titles.
 */
function similarterms_similarterms_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s": Similar by terms', array('@s' => $context->identifier));
}

/**
 * Returns an edit form for the content type.
 */
function similarterms_similarterms_content_type_edit_form(&$form, &$form_state) {
  // Pull out current settings from the form state.
  $conf =(is_array($form_state['conf'])) ? $form_state['conf'] : array();

  // Merge in default settings.
  $conf += array(
    'vocabulary' => 0,
    'display_options' => 'title_only',
  );

  $vocabulary_options = array(0 => t('All vocabularies'));
  foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
    $vocabulary_options[$vid] = $vocab->name;
  }
  $form['vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#description' => t('Which vocabulary should be used to calculate the similar items?'),
    '#options' => $vocabulary_options,
    '#required' => TRUE,
    '#default_value' => $conf['vocabulary'],
  );

  $form['display_options'] = array(
    '#type' => 'radios',
    '#title' => t('Display Options'),
    '#default_value' => $conf['display_options'],
    '#options' =>  array(
      'title_only' => t('Display titles only'),
      'teaser' => t('Display titles and teaser'),
    ),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Submit handler for the content type editing form.
 */
function similarterms_similarterms_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['vocabulary'] = $form_state['values']['vocabulary'];
  $form_state['conf']['display_options'] = $form_state['values']['display_options'];
}

