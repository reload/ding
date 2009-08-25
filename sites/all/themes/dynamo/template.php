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


// $form['submit']['#type'] 	= "image_button" ;
// $form['submit']['#src'] 	= drupal_get_path('theme','mdkate')."/images/foo.gif";
// $form['submit']['#attributes']['class'] 	= "";



function dynamo_preprocess_ting_collection__FOO(&$variables){
	
	foreach ($variables['collection']->objects as $key => $value) {
		$output[$key]['id'] = $variables['collection']->objects[$key]->id;
		$output[$key]['title'] = $variables['collection']->objects[$key]->data->title[0];
		$output[$key]['desciption'] .= $variables['collection']->objects[$key]->data->description[0];
		$output[$key]['date'] = $variables['collection']->objects[$key]->data->date[0];
		$output[$key]['type'] = $variables['collection']->objects[$key]->data->type[0];
		$output[$key]['format'] = $variables['collection']->objects[$key]->data->format[0];
		$output[$key]['source'] = $variables['collection']->objects[$key]->data->source[0];


		for ($i=0; $i < count($variables['collection']->objects[$key]->data->description) ; $i++) { 
			$output[$key]['description'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->description[$i] .'</span>';
		}


		//creator
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->creator) ; $i++) { 
			$output[$key]['creator'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->creator[$i] .'</span>';
		}

		//subject
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->subject) ; $i++) { 
			$output[$key]['subject'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->subject[$i] .'</span>';
		}

		//publisher
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->publisher) ; $i++) { 
			$output[$key]['publisher'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->publisher[$i] .'</span>';
		}

		//contributor
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->contributor) ; $i++) { 
			$output[$key]['contributor'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->contributor[$i] .'</span>';
		}

		//language
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->language) ; $i++) { 
			$output[$key]['language'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->language[$i] .'</span>';
		}

		//relation
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->relation) ; $i++) { 
			$output[$key]['relation'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->relation[$i] .'</span>';
		}

		//coverage
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->coverage) ; $i++) { 
			$output[$key]['coverage'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->coverage[$i] .'</span>';
		}

		//rights
		for ($i=0; $i < count($variables['collection']->objects[$key]->data->rights) ; $i++) { 
			$output[$key]['rights'][$i]	= '<span>' . $variables['collection']->objects[$key]->data->rights[$i] .'</span>';
		}

	}

	$variables['collection_data'] = $output;
}


//views
function dynamo_preprocess_views_view_list(&$vars){
  dynamo_preprocess_views_view_unformatted($vars);  
}

  function dynamo_preprocess_views_view_unformatted(&$vars) {
    $view     = $vars['view'];
    $rows     = $vars['rows'];

    $vars['classes'] = array();
    // Set up striping values.
     foreach ($rows as $id => $row) {
    //  $vars['classes'][$id] = 'views-row-' . ($id + 1);
    //    $vars['classes'][$id] .= ' views-row-' . ($id % 2 ? 'even' : 'odd');
      if ($id == 0) {
        $vars['classes'][$id] .= ' first';
      }
    }
    $vars['classes'][$id] .= ' last';
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


function dynamo_username($object) {

//	 (!empty($user->alma_name)), 
  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($object->name) > 20) {
      $name = drupal_substr($object->name, 0, 15) .'...';
    }
    else {
			//alma name
	 		if(!empty($object->alma_name)){
		 		$name = $object->alma_name;
			}else{
				$name = $object->name;
			} 			
     
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/'. $object->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
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

/*
* format_danmarc2
	documentation http://www.kat-format.dk/danMARC2/Danmarc2.5c.htm#pgfId=1575053
*/
function format_danmarc2($string){

	$string = str_replace('Indhold:','',$string);	
	$string = str_replace(' ; ','<br/>',$string);	
	$string = str_replace(' / ','<br/>',$string);	 

	return $string;
}

function dynamo_comment_form($form){
//	dsm($form);
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
