<?php 

/**
 * An implementation of hook_theme_registry_alter()
 * Iterate over theme registry, injecting our catch function into every theme call, including template calls.
 * The catch function logs theme calls and performs divine nastiness.
 *
 * @return void
 **/

function devel_themer_comments_theme_registry_alter($theme_registry) {
	foreach ($theme_registry as $hook => $data) {
	  if (isset($theme_registry[$hook]['function'])) {
	    // If the hook is a function, store it so it can be run after it has been intercepted.
	    // This does not apply to template calls.
	    $theme_registry[$hook]['devel_function_intercept'] = $theme_registry[$hook]['function'];
	  }
	  // Add  our catch function to intercept functions as well as templates.
	  $theme_registry[$hook]['function'] = 'devel_themer_comments_catch_function';
	}
}

/**
 * Intercepts all theme calls (including templates), adds to template log, and dispatches to original theme function.
 * This function gets injected into theme registry in devel_themer_theme_registry_alter().
 */
function devel_themer_comments_catch_function() {

  $args = func_get_args();

  // Get the function that is normally called.
  $trace = debug_backtrace();
  $hook = $trace[2]['args'][0];
  array_unshift($args, $hook);

  $counter = devel_counter();
  $timer_name = "thmr_$counter";
  timer_start($timer_name);

  // The twin of theme(). All rendering done through here.
  list($return, $meta) = call_user_func_array('devel_themer_theme_twin', $args);
  $time = timer_stop($timer_name);

  $skip_calls = array('hidden', 'form_element', 'placeholder');

  if (!empty($return) && !is_array($return) && !is_object($return) && user_access('access devel information')) {
	
    if (!in_array($hook, $skip_calls) && empty($GLOBALS['devel_themer_stop'])) {
      if ($meta['type'] == 'func') {
	      $name = $meta['used'];
	      $used = $meta['used'];
	      if (empty($meta['wildcards'])) {
	        $meta['wildcards'][$hook] = '';
	      }  
	      $candidates = devel_themer_ancestry(array_reverse(array_keys($meta['wildcards'])));
	      if (empty($meta['variables'])) {
	        $variables = array();
	      }
      }else{
        $name = $meta['used']. devel_themer_get_extension();
        if (empty($suggestions)) {
          array_unshift($meta['suggestions'], $meta['used']);
        }
        $candidates = array_reverse(array_map('devel_themer_append_extension', $meta['suggestions']));
        $used = $meta['template_file'];
      }

      $key = "thmr_$counter";
      // This variable gets sent to the browser in Drupal.settings.
      $GLOBALS['devel_theme_calls'][$key] = array(
        'name' => $name,
        'type' => $meta['type'],
        'duration' => $time['time'],
        'used' => $used,
        'candidates' => $candidates,
        'preprocessors' => isset($meta['preprocessors']) ? $meta['preprocessors'] : array(),
      );
		    
      // This variable gets serialized and cached on the server.
      $GLOBALS['devel_themer_server'][$key] = array(
        'variables' => $meta['variables'],
        'type' => $meta['type'],
      );
		
    if ($hook == 'page' ) {
      $GLOBALS['devel_theme_calls']['page_id'] = $counter;
      // Stop logging theme calls after we see theme('page'). This prevents
      // needless logging of devel module's query log, for example. Other modules may set this global as needed.
      $GLOBALS['devel_themer_stop'] = TRUE;
    }else {
			$output .= "\n<!-- - - - - - - - - - - thmr ".$GLOBALS['devel_theme_calls'][$key][name]." - - - - - - - - - - -->\n";

			//function or tpl filed called
			if($GLOBALS['devel_theme_calls'][$key][type] == "tpl"){
				$output .= "<!-- tpl: ".$GLOBALS['devel_theme_calls'][$key][name] ." -->\n" ;
				$output .= "<!-- file: ". $GLOBALS['devel_theme_calls'][$key][used] ." -->\n" ;
			}else{
//				$output .= "<!--function called: ".$GLOBALS['devel_theme_calls'][$key][name] ." -->\n" ;
			}
/*
			//candidates
			if($GLOBALS['devel_theme_calls'][$key][type] == "tpl"){
				$output .= "<!--  candidate files: -->\n";
			}else{
				$output .= "<!--  candidate function:  -->\n";			
			}

			$c = count($GLOBALS['devel_theme_calls'][$key][candidates]);
			for ($i=0; $i < $c; $i++){
				$output .= "<!--\t * ". $GLOBALS['devel_theme_calls'][$key][candidates][$i] ."-->\n" ;
			}			

			//preprocess
			$c = count($GLOBALS['devel_theme_calls'][$key][preprocessors]);
			if($c){
				$output .= "<!-- preproces functions: -->\n";
			}
			for ($i=0; $i < $c; $i++) { 
				$output .= "<!-- \t* ". $GLOBALS['devel_theme_calls'][$key][preprocessors][$i] ."  -->\n" ;		
			}			
			//duration
			$output .= "<!--Duration: ". $GLOBALS['devel_theme_calls'][$key][duration] ."ms -->\n" ;		
*/
			//content
			$output .= $return.   "\n"; //$suffix.

			$output .= "\n<!-- - - - - - - - - - - / thmr ".$GLOBALS['devel_theme_calls'][$key][name]." - - - - - - - - - - -->\n\n";

    }


    }else{
     	$output = $return;
  	}

  }


  return isset($output) ? $output : $return;


}
