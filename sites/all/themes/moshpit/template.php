<?php
/* =====================================
  template.php
* ------------------------------------- */

// Auto-rebuild the theme registry during theme development.
drupal_rebuild_theme_registry(); /*TODO: add a theme setting for this*/


/**
 * Implements theme_menu_item_link()
 * yup original is from zen and we humbly say thanx :)
 */
function moshpit_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }

  //define a class for each tab 
  //TODO check out the multilang hell....
  $linkclass = mothership_id_safe($link['title']);
  
  // If an item is a LOCAL TASK, render it as a tab
  if($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab '. $linkclass .' ">' . check_plain($link['title']) . '</span>';
    $link['localized_options']['html'] = TRUE;
    $link['localized_options']['attributes'] = array('class' => $linkclass);
  }else{
    // its a standard menu item lets kick in a span class
    $link['title'] = '<span class="icon-'. $linkclass .' ">' . check_plain($link['title']) . '</span>';
    $link['localized_options']['html'] = TRUE;
    $link['localized_options']['attributes'] = array('class' => $linkclass);
  }
  
  return l($link['title'], $link['href'], $link['localized_options']);
}

/*adding a top level to the nice menu*/
function moshpit_nice_menu_build($menu, $level = "0") {
  $output = '';
  /*new class*/
  $level_class = "nice-menu-level-".$level.' ';

  foreach ($menu as $menu_item) {
    $mlid = $menu_item['link']['mlid'];
    // Check to see if it is a visible menu item.

    if ($menu_item['link']['hidden'] == 0) {
      // Build class name based on menu path
      // e.g. to give each menu item individual style.
      // Strip funny symbols.
      $clean_path = str_replace(array('http://', '<', '>', '&', '=', '?', ':'), '', $menu_item['link']['href']);
      // Convert slashes to dashes.
      $clean_path = str_replace('/', '-', $clean_path);
      $path_class = 'menu-path-'. $clean_path;

      // If it has children build a nice little tree under it.
      if ((!empty($menu_item['link']['has_children'])) && (!empty($menu_item['below']))) {
        // Keep passing children into the function 'til we get them all.
        $children = theme('nice_menu_build', $menu_item['below'],'1');
        // Set the class to parent only of children are displayed.
        $parent_class = $children ? 'menuparent ' : '';
        $output .= '<li id="menu-'. $mlid .'" class=" '.$level_class . $parent_class . $path_class .'">'. theme('menu_item_link', $menu_item['link']);
        // Build the child UL only if children are displayed for the user.
        if ($children) {
          $output .= '<ul class="' . $level_class . '">';
          $output .= $children;
          $output .= "</ul>\n";
        }
        $output .= "</li>\n";
      }
      else {
        $output .= '<li id="menu-'. $mlid .'" class="'. $level_class . $path_class .'">'. theme('menu_item_link', $menu_item['link']) .'</li>'."\n";
      }
			
    }

  }
  return $output;
}


