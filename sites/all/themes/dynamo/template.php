<?php
/* =====================================
  Dynamo
  template.php
* ------------------------------------- */
/**
* Implementation of hook_theme().
*/
function dynamo_theme($existing, $type, $theme, $path) {
 return array(
   'ding_panels_content_library_title' => array(
     'template' => 'ding_panels_content_libary_title',     
   ),
   'ding_panels_content_library_location' => array(
     'template' => 'ding_panels_content_libary_location',
   ),
	'ting_search_form' => array(
		'arguments'=> array('form' => NULL),
		),
	'user_login_block' => array(
		'arguments' => array ('form' => NULL),
		),

	'comment_form' => array(
		'arguments' => array ('form' => NULL),
		),

 );
}

/**
 * Preprocess node template variables.
 */
function dynamo_preprocess_node(&$variables) {
  $node = $variables['node'];
  if (!$variables['page']) {
    if (isset($variables['field_list_image_rendered']) && strlen($variables['field_list_image_rendered']) > 1) {
      $variables['list_image'] = $variables['field_list_image_rendered'];
    }
    else {
      $variables['list_image'] = '&nbsp;'; //<--why ??
    }
  }

  $similar_nodes = similarterms_list(variable_get('ding_similarterms_vocabulary_id', 0));
  if (count($similar_nodes)) {
    $variables['similarterms'] = theme('similarterms', variable_get('similarterms_display_options', 'title_only'), $similar_nodes);
  }

  if ($variables['type'] == 'event') {
    $start = strtotime($node->field_datetime[0]['value']);
    $end = strtotime($node->field_datetime[0]['value2']);

    // If no end time is set, use the start time for comparison.
    if (2 > $end) {
      $end = $start;
    }

    // Find out the end time of the event. If there's no specified end
    // time, we’ll use the start time. If the event is in the past, we
    // create the alert box.
    if (($end > 0 && date('Ymd', $end) < date('Ymd', $_SERVER['REQUEST_TIME']))) {
      $variables['alertbox'] = '<div class="alert">' . t('NB! This event occurred in the past.') . '</div>';
    }

    // More human-friendly date formatting – try only to show the stuff
    // that’s different when displaying a date range.
    if(date("Ymd", $date) == date("Ymd", $end)) {
      $variables['event_date'] = format_date($start, 'custom', "j. F Y");
    }
    elseif(date("Ym", $date) == date("Ym", $end)) {
      $variables['event_date'] = format_date($start, 'custom', "j.") . "–" . format_date($end, 'custom', "j. F Y");
    }
    else {
      $variables['event_date'] = format_date($start, 'custom', "j. M.") . " – " . format_date($end, 'custom', "j. M. Y");
    }

    // Display free if the price is zero.
    if ($node->field_entry_price[0]['value'] == "0") {
      $variables['event_price'] = t('free');
    }
    else{
      $variables['event_price'] = filter_xss($node->field_entry_price[0]['view']);
    }
  }
}

/**
 * Implementation of theme_breadcrumb().
 */
function dynamo_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    // Remove the last, empty item from the breadcrumb trail.
    if (end($breadcrumb) == NULL) {
      array_pop($breadcrumb);
    }
    // Append the page title to the breadcrumb.
    $breadcrumb[] = menu_get_active_title();
    $output = '<div id="path">' . t('You are here:');
    $output .= ' <div class="breadcrumb">'. implode(' » ', $breadcrumb) .'</div>';
    $output .= '</div>';
    return $output;
  }
}

/*forms*/
function dynamo_user_login_block($form){
	$form['submit']['#type'] 	= "image_button" ;
	$form['submit']['#src'] 	= drupal_get_path('theme','dynamo')."/images/accountlogin.png";
	$form['submit']['#attributes']['class'] 	= "";


	$name = drupal_render($form['name']); 
	$pass =  	drupal_render($form['pass']); 
	$submit =  	drupal_render($form['submit']); 
	$remember =	drupal_render($form['remember_me']); 
	
	return 	$name . $pass .$submit . $remember . drupal_render($form);
}

function dynamo_ting_search_form($form){
	$form['submit']['#type'] 	= "image_button" ;
	$form['submit']['#src'] 	= drupal_get_path('theme','dynamo')."/images/searchbutton.png";
	$form['submit']['#attributes']['class'] 	= "";

	return drupal_render($form);	
}

function dynamo_comment_form($form){
	$form['comment_filter']['format']['#collapsed'] = FALSE;
	unset($form['notify_clearit']);
	unset($form['comment_filter']['format']);
	// $form['_author']['#value'] = '<span>' . $form['_author']['#value'] .'</span>'; // adds a span around
	// $form['submit']['#type'] 	= "image_button" ;
	// $form['submit']['#src'] 	= drupal_get_path('theme','mdkate')."/images/foo.gif";
	// $form['submit']['#attributes']['class'] 	= "";

	$submit = drupal_render($form['submit']);
	$preview = drupal_render($form['preview']);
	$theform = drupal_render($form);
	return  $theform .'<div class="form-buttons">' . $submit . $preview .'</div>';

	return drupal_render($form);
}



/**
 * office hours
 */
function dynamo_office_hours_format_day($name, $values, $day_number) {
  $oddity = ($day_number % 2) ? 'odd' : 'even';
  $output = '<div class="' . $oddity . '">';
  $output .= '<span class="day">' . $name . '</span>';
  if (is_array($values) && !empty($values)) {
    foreach ($values as $val) {
      $output .= ' <span class="hours start">' . _office_hours_format_time($val['start']) . '</span>';
      $output .= ' – <span class="hours end">' . _office_hours_format_time($val['end']) . '</span>';
    }
  }
  else {
    $output .= ' <span class="closed">' . t('closed') . '</span>';
  }
  return $output . '</div>';
}


/*
* panels
*/
function dynamo_panels_pane($content, $pane, $display) {
  if (!empty($content->content)) {
    $idstr = $classstr = '';
    if (!empty($content->css_id)) {
      $idstr = ' id="' . $content->css_id . '"';
    }
    if (!empty($content->css_class)) {
      $classstr = ' ' . $content->css_class;
    } 
    //  $output = "<div class=\"panel-pane $classstr\"$idstr>\n";
    $output = "<div class=\"panel-pane pane-$pane->subtype $classstr \"$idstr>\n";
    if (!empty($content->title)) {
      $output .= "<h3>$content->title</h3>\n";
    }

    if (!empty($content->feeds)) {
      $output .= "<div class=\"feed\">" . implode(' ', $content->feeds) . "</div>\n";
    }

    //  $output .= "<div class=\"content\">$content->content</div>\n";
    $output .= $content->content;

    if (!empty($content->links)) {
      $output .= "<div class=\"links\">" . theme('links', $content->links) . "</div>\n";
    }

    if (!empty($content->more)) {
      if (empty($content->more['title'])) {
        $content->more['title'] = t('more');
      }
      $output .= "<div class=\"panels more-link\">" . l($content->more['title'], $content->more['href']) . "</div>\n";
    }
    if (user_access('view pane admin links') && !empty($content->admin_links)) {
      $output .= "<div class=\"admin-links panel-hide\">" . theme('links', $content->admin_links) . "</div>\n";
    }


    $output .= "</div>\n";
    return $output;
  }
}


function dynamo_panels_default_style_render_panel($display, $panel_id, $panes, $settings) {
  $output = '';

  $print_separator = FALSE;
  foreach ($panes as $pane_id => $content) {
    // Add the separator if we've already displayed a pane.
    if ($print_separator) {
     // $output .= '<div class="panel-separator"></div>';
    }
    $output .= $text = panels_render_pane($content, $display->content[$pane_id], $display);

    // If we displayed a pane, this will become true; if not, it will become
    // false.
    $print_separator = (bool) $text;
  }

  return $output;
}


//Taxonomy
//returns the terms from a given  vocab
function return_terms_from_vocabulary($node, $vid){
  $terms = taxonomy_node_get_terms_by_vocabulary($node, $vid, $key = 'tid');

//	$vocabolary = taxonomy_get_vocabulary($vid);
  $vocabolary = taxonomy_vocabulary_load($vid);

//	$content ='<div class="vocabolary_terms">';
//	$content .='<div class="vocabolary">'.$vocabolary->name.'</div>';
		$termslist = '';
		if ($terms) {
			$content .= '<div class="terms">';
			foreach ($terms as $term) {
				$termslist = $termslist.l($term->name, 'taxonomy/term/'.$term->tid) . ' | ';
			//	$termslist = $termslist .$term->name  .' | ';			
			}
			$content.= trim ($termslist," |").'</div>';
		}
//	$content.='</div>';

	return $content;
}


/**
 * Implementation of theme_username().
 */
function dynamo_username($object) {
  // We might get passed node objects or other strangeness, so if object
  // doesn’t look like a user, try and load the user instead.
  if ($object->uid && (!isset($object->pass) || !isset($object->login))) {
    $account = user_load($object->uid);
    if ($account) {
      $object = $account;
    }
  }

  if ($object->uid && $object->name) {
    if (!empty($object->display_name)) {
      $name = $object->display_name;
    }
    else {
      $name = $object->name;
    }

    // Prevent extremely long names of non-trusted users from breaking the
    // design.
    if (drupal_strlen($name) > 20 && empty($object->has_secure_role)) {
      $name = drupal_substr($name, 0, 15) .'…';
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/'. $object->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      $output = check_plain($name);
    }
  }
  elseif ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if (!empty($object->homepage)) {
      $output = l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow')));
    }
    else {
      $output = check_plain($object->name);
    }

    $output .= ' ('. t('not verified') .')';
  }
  else {
    $output = check_plain(variable_get('anonymous', t('Anonymous')));
  }

  return $output;
}

/**
 * Crudely format danMARC2 data.
 *
 * Documentation: http://www.kat-format.dk/danMARC2/Danmarc2.5c.htm#pgfId=1575053
 */
function format_danmarc2($string){
	$string = str_replace('Indhold:','',$string);	
	$string = str_replace(' ; ','<br/>',$string);	
	$string = str_replace(' / ','<br/>',$string);	 

	return $string;
}
