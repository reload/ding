<?php
// $Id$

/**
 * @file ding_library_map.tpl.php
 * Library location, map, etc.
 */

//TODO: Move config to theme preprocess
drupal_add_css(drupal_get_path('module', 'ding_library_map') .'/css/ding_library_map.css', 'module');
drupal_add_js(drupal_get_path('module', 'ding_library_map') .'/js/jquery.scrollTo/jquery.scrollTo-min.js', 'module');
drupal_add_js(drupal_get_path('module', 'ding_library_map') .'/js/jquery.url/jquery.url.packed.js', 'module');
drupal_add_js(drupal_get_path('module', 'ding_library_map') .'/js/ding_library_map.js', 'module');

//setup map
$mapId = 'library_map';
$map = array('id' => $mapId, 'type' => 'map', 'zoom' => 12, 'minzoom' => 9, 'maxzoom' => 14, 'height' => '200px', 'width' => '100%', 'controltype' => 'Small', 'behavior' => array('extramarkerevents' => 1, 'nomousezoom' => 1));
if ($center = variable_get('ding_library_map_center', false)) {
  $center = explode(',', $center, 2);
  $variables = array(0 => 'latitude', 1 => 'longitude');
  foreach ($variables as $index => $attribute) {
	  if (isset($center[$index]) && is_numeric($center[$index])) {
	    $map[$attribute] = trim($center[$index]);
	  }
  }
}

//add markers for libraries
foreach ($nodes as $node) {
	$libraryId = ($node->ding_slug) ? $node->ding_slug : $node->nid;
	$marker = array('latitude' => $node->location['latitude'], 
									'longitude' => $node->location['longitude'], 
									'markername' => 'ding_library_map_'.$node->field_opening_hours_processed['status'],
									'name' => $node->title,
									'street' => $node->location['street'], 
									'city' => $node->location['city'], 
									'postal-code' => $node->location['postal_code'],
									'opening_hours' => $node->field_opening_hours_processed['week'],
									'state' => $node->field_opening_hours_processed['status'], 
									'url' => url('biblioteker/'.$libraryId, array('absolute' => TRUE)),
									'text' => FALSE);
	$map['markers'][] = $marker;
}

?>
<div class="ding-library-map">
	<?php echo theme('gmap', $map) ?>
	<script type="text/javascript">
		<?php //TODO: Move to Drupal javascript settings ?>
		var options = { fullDayNames: {	'mon': '<?php echo t('Monday') ?>',
										 								'tue': '<?php echo t('Tuesday') ?>',
																		'wed': '<?php echo t('Wednesday') ?>',
																		'thu': '<?php echo t('Thursday') ?>',
																		'fri': '<?php echo t('Friday') ?>',
																		'sat': '<?php echo t('Saturday') ?>',
																		'sun': '<?php echo t('Sunday') ?>' },
										shortDayNames: {	'mon': '<?php echo t('Mon') ?>',
							 		 										'tue': '<?php echo t('Tue') ?>',
											 								'wed': '<?php echo t('Wed') ?>',
											 								'thu': '<?php echo t('Thu') ?>',
											 								'fri': '<?php echo t('Fri') ?>',
											 								'sat': '<?php echo t('Sat') ?>',
											 								'sun': '<?php echo t('Sun') ?>' }
		};
									
		jQuery(function()
		{
			Drupal.dingLibraryMap('<?php echo $mapId; ?>', options)
		});
	</script>
</div>