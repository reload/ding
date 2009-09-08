<?php
// $Id$

/**
 * Admin settings form.
 */
function production_site_admin_settings_form($form_state) {
  $form = array();

  $form['production_site_list_type'] = array(
    '#type' => 'radios',
    '#description' => t('In blacklist mode, the following sites will NOT be considered production sites, if in whitelist mode, they will, and all other sites will not.'),
    '#title' => t('Rule type'),
    '#options' => array(
      FALSE => t('Blacklist'),
      TRUE => t('Whitelist'),
    ),
    '#default_value' => variable_get('production_site_rule_inclusive', FALSE),
  );

  $form['production_site_hostnames'] = array(
    '#type' => 'textarea',
    '#title' => t('Host names'),
    '#description' => t('Host names that should be evaluated as when determining if a site is production or development, one per line with no extra punctuation. These support partial matches, so that “dev.gnit.dk” will also match “kkb.dev.gnit.dk” and “asdf.flaf.dev.gnit.dk”.'),
    '#default_value' => variable_get('production_site_hostnames', ''),
  );

  return system_settings_form($form);
}

