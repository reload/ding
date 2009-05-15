<?php
// $Id$

/**
 * @file ding_panels_content.theme.inc
 * Theming implementation for library panes.
 */

/**
 * Preprocess hook for the library title and navigation pane.
 */
function ding_panels_content_preprocess_library_title(&$variables) {
  $node = $variables['node'];
  $base_path = url('node/' . $node->nid);

  $variables['library_title'] = l($node->title, $base_path);

  if (!isset($node->library_page_type) || $node->library_page_type == 'normal') {
    $variables['library_links'] = _ding_panels_content_theme_library_title_navigation($node, $base_path);
    $variables['library_navigation'] = theme('item_list', $variables['library_links']);
  }
}

function _ding_panels_content_theme_library_title_navigation($node, $base_path) {
  $links = array();
  if ($node->type == 'library') {
    $links['blog'] = l(t('Blog'), $base_path . '/blog');
    $links['events'] = l(t('Events'), $base_path . '/arrangementer');
    $links['facilities'] = l(t('Facilities'), $base_path . '/faciliteter');
    $links['about'] = l(t('About'), $base_path . '/om');
  }
  return $links;
}

/**
 * Preprocess hook for the library location.
 */
function ding_panels_content_preprocess_library_location(&$variables) {
  $node = $variables['node'];

  // Add a static Google map to the location information.
  if (function_exists('location_has_coordinates') && location_has_coordinates($node->location)) {
    $map_url = url('http://maps.google.com/staticmap', array('query' => array(
      'zoom' => 14,
      'size' => '194x140',
      'markers' => $node->location['latitude'] . ',' . $node->location['longitude'],
      'key' => keys_api_get_key('gmap', $_SERVER['HTTP_HOST']),
      'sensor' => 'false',
    )));
    $variables['library_map'] = theme('image', $map_url, '', '', NULL, FALSE);
  }
}

/**
 * Theme function for library portal title.
 */
function theme_ding_panels_library_portal_title($node) {
  return '<h1>' . check_plain($node->title) . '</h1>';
}

/**
 * Preprocess function for the staff title.
 */
function ding_panels_content_preprocess_library_user_staff_title(&$variables) {
  // Generate list of links to the libraries the user is related to.
  $libs = array();
  if (isset($variables['user']->userreference['library'])) {
    foreach ($variables['user']->userreference['library'] as $nid => $title) {
      $libs[] = l($title, 'node/' . $nid);
    }
  }
  if (!empty($libs)) {
    $variables['related_libraries'] = theme('item_list', $libs);
  }
  else {
    $variables['related_libraries'] = '';
  }
}

