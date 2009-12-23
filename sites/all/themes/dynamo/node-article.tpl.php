<?php
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

<!--node- article.tpl-->
<div<?php print $id_node . $classes; ?>>
  <div class="subject">
    <?php print return_terms_from_vocabulary($node, "1"); ?> 
  </div>

	<?php if($node->title){	?>	
	  <h2><?php print $title;?></h2>
	<?php } ?>

	<div class="meta">
	  
		<?php print format_date($node->created, 'custom', "j F Y") ?> 
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
		

  <?php if ($similarterms) { ?>
    <div class="ding-box-wide similar">
      <h3><?php print t('Similar'); ?></h3>
      <?php print $similarterms; ?>
    </div>
  <?php } ?>


	<?php if ($links){ ?>
    <?php  print $links; ?>
	<?php } ?>

</div>
<!-- /node.tpl-->
