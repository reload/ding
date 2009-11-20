<?php
// $Id$

/**
 * @file ting.theme.inc
 * Theming functions for Ting objects, etc.
 */

/**
 * Theme a Ting object in title form.
 *
 * @param TingClientObject $ting_object
 *    The object we're theming.
 * @param boolean $display_image
 *    Output the objects cover image in front of the title, if available. 
 * @return string
 *    Rendered HTML.
 */
function theme_ting_object_title($ting_object, $display_image = FALSE) {
  $output = '';
  if ($ting_object) {
    if ($display_image) {
      $image_url = ting_covers_object_url($ting_object, '80_x');
      if ($image_url) {
        $output .= theme('image', $image_url, '', '', NULL, FALSE) . $loan_title;
      }
    }
    $output .= '<span class="title">' . l($ting_object->data->title[0], 'ting/object/' . $ting_object->id) . '</span>';

  }
  else {
    return t('Title not available.');
  }
  return $output;
}

