<div class="campaign campaign-theme-<?php print $node->campaign_theme;?> campaign-type-<?php print $node->campaign_type;?> clearfix">
<?php 
if ($node->campaign_type == "image-only") { 
  print l($node->field_campaign_image['0']['view'], $node->field_campaign_link['0']['display_url'], $options= array('html'=>TRUE));
}else{ 
  print l(filter_xss($node->content['body']['#value']),  $node->field_campaign_link['0']['display_url'], $options= array('html'=>TRUE));
} 
?>
</div>