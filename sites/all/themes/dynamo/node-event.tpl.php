<?php
/*
	dsm($variables['template_files']);
  dsm($node);
  dsm($node->content);
  print_r(get_defined_vars());
  print $FIELD_NAME_rendered;
*/
/*
ad a class="" if we have anything in the $classes var
this is so we can have a cleaner output - no reason to have an empty <div class="" id=""> 
*/
if($classes){
   $classes = ' class="' . $classes . ' clearfix"';
}

if($id_node){
  $id_node = ' id="' . $id_node . '"';  
}
?>
<!-- node-event.tpl-->
<?php if ($page == 0){ ?>
<div<?php print $id_node . $classes; ?>>

  <div class="picture">
    <?php print $field_image_rendered; ?>
  </div>

  <div class="content">

  	<?php if($node->title){	?>	
      <h3><?php print l($node->title, 'node/'.$node->nid); ?></h3>
  	<?php } ?>

  	<div class="meta">
      <?php print $field_datetime_rendered ?>
      <?php print $field_library_ref_rendered ?>          
        
  		<?php if (count($taxonomy)){ ?>
  		  <div class="taxonomy">
  	   	  <?php print $terms ?> 
  		  </div>  
  		<?php } ?>

      <?php print $field_entry_price_rendered ?>
  	</div>

    <?php print $node->content['body']['#value'];?>
    
  </div>

</div>
<?php }else{ 
//Content
?>
<div<?php print $id_node . $classes; ?>>
	<?php if($node->title){	?>	
	  <h1><?php print $title;?></h1>
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

	<div class="content">
		<?php print $content ?>
	</div>
		
	<?php if ($links){ ?>
    <?php  print $links; ?>
	<?php } ?>
</div>
<?php } ?>