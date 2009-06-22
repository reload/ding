<?php // dsm($node);	?>	
<?php  //dsm(get_defined_vars());  ?> 
<?php //print $FIELD_NAME_rendered ?>


<div class="clearfix campaign campaign-theme-<?php print $node->campaign_theme;?> campaign-type-<?php print $node->campaign_type;?>">

<?php if ($node->campaign_type == "image-only") { ?>
    <?php
      print l($node->field_campaign_image['0']['view'], $node->field_campaign_link['0']['display_url'], $options= array('html'=>TRUE));
   ?>

<?php }else{ ?>

  <?php if($node->title){	?>	
    <h3><?php print l($node->title, 'node/'.$node->nid); ?></h3>
  <?php } ?>

<?php
    print l($node->teaser,  $node->field_campaign_link['0']['display_url'], $options= array('html'=>TRUE));

?>  


<?php } ?>

  </div>