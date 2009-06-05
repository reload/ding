<?php
// $Id$

/**
 * Variable preprocessing for the ding_event_similar_events template.
 */
function ding_event_similar_events_preprocess($variables) {
  $variables['events'] = array();
  // Load the node for each event, so Morten can theme them :)
  foreach ($variables['event_nids'] as $nid) {
    $variables['events'][$nid] = node_load($nid);
  } 
}

