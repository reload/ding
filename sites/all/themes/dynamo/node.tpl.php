<?php //dsm($node);	?>	
<?php  //dsm(get_defined_vars());  ?> 
<?php //print $FIELD_NAME_rendered ?>
<!-- node.tpl-->
<?php if ($page == 0){ ?>
<div class="<?php print $classes ?> clearfix">

  <div class="picture">
    <?php print $field_image_rendered; ?>
  </div>

  <div class="content">

    <div class="subject">
      <?php print return_terms_from_vocabulary($node, "1"); ?> 
    </div>


  	<?php if($node->title){	?>	
      <h3><?php print l($node->title, 'node/'.$node->nid); ?></h3>
  	<?php } ?>

  	<div class="meta">
  		<span class="time">
  			<?php print format_date($node->created, 'custom', "j F Y") ?> 
  		</span>	
  		<span class="author">
  			af <?php print theme('username', $node); ?>
  		</span>	

  		<?php if (count($taxonomy)){ ?>
  		  <div class="taxonomy">
  	   	  <?php print $terms ?> 
  		  </div>  
  		<?php } ?>
  	</div>

    <?php print $node->content['body']['#value'];?>
    
  </div>

</div>
<?php }else{ 
//Content
?>
<div class="<?php print $classes ?>">

  <div class="subject">
    <?php print return_terms_from_vocabulary($node, "1"); ?> 
  </div>

	<?php if($node->title){	?>	
	  <h1><?php print $title;?></h1>
	<?php } ?>
		
	<div class="meta">
	  
		<?php print format_date($node->created, 'custom', "j F Y - H:i") ?> 
    <i><?php print t('by'); ?></i> 
		<span class="author"><?php print theme('username', $node); ?></span>	
	</div>

	<div class="content">
		<?php print $content ?>
	</div>

	<?php if (count($taxonomy)){ ?>
	  <div class="taxonomy">
   	  <?php print $terms ?> 
	  </div>  
	<?php } ?>
		
	<?php if ($links){ ?>
    <?php  print $links; ?>
	<?php } ?>
</div>
<?php } ?>