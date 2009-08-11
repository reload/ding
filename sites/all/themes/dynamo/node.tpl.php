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

<!-- node.tpl-->
<?php if ($page == 0){ ?>
<div<?php print $id_node . $classes; ?>>

  <div class="picture">
    <?php
 		if($field_image_rendered){
			print $field_image_rendered; 	
		}
	?>
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

			<?php print $node->field_library_ref[0]['view'];  ?>

			
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
<div<?php print $id_node . $classes; ?>>

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

  <?php $similar_nodes = similarterms_list(variable_get('ding_similarterms_vocabulary_id', 0)); ?>
  <?php if (count($similar_nodes)) { ?>
	  <div class="similar">
      <h3><?php print t('Similar'); ?></h3>
      <?php print theme('similarterms', variable_get('similarterms_display_options', 'title_only'), $similar_nodes); ?>
    </div>
  <?php } ?>
</div>
<?php } ?>
<!-- /node.tpl-->