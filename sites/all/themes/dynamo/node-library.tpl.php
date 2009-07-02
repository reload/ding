<?php  //dsm(get_defined_vars());  ?> 
<?php //print $FIELD_NAME_rendered ?>
<?php 
//  if($node->nid=="1"){
 //   dsm(get_defined_vars());
//  dsm($node);
//  }

?>
<?php if ($page == 0){ ?>
<!--node-lib-->
<div class="clearfix node-library-teaser node-teaser">

  <div class="picture">
    <?php print $field_image_rendered; ?>
  </div>

  <div class="content">
  
    <div class="libary-openstatus <?php print $node->field_opening_hours_processed['status'];?>">
        <?php print t($node->field_opening_hours_processed['status']);?>
    </div>

    <div class="vcard">

      <h3 class="fn org"><?php print l($node->title, 'node/'.$node->nid); ?></h3>
      <div class="adr">
        <div class="street-address"><?php print $node->location['street']; ?></div>
        <span class="postal-code"><?php print $node->location['postal_code']; ?></span>
        <span class="locality"><?php print $node->location['city']; ?></span>

      </div>

      <div class="link-card">
          <a href="" id="biblo-<?php print $node->nid ?>">Se på kort</a>
      </div>
      
      <div class="tel">
        <span class="type"><?php print t('Phone'); ?>:</span> <span><?php print $node->location['phone']; ?></span>
      </div>
      <div class="tel">
        <span class="type"><?php print t('Fax'); ?>:</span> <span><?php print $node->location['fax']; ?></span>
      </div>
      <div class="email">
        <span class="type"><?php print t('Fax'); ?>:</span> <span><?php print $node->field_email['0']['view']; ?></span>
      </div>

    </div>



    








  </div>
  <?php print $node->field_opening_hours['0']['view'];?>
</div>

<?php }else{ 
//Content
?>
<div class="<?php print $classes ?>">

	<h1><?php print $title;?></h1>
		
	<div class="meta">
  	<?php if ($picture) { ;?>
			<span class="author-picture">
		  		<?php print $picture; ?>  
			</span>
		<?php } ?>


		<span class="time">
			<?php print format_date($node->created, 'custom', "j F Y") ?> 
		</span>	
		<span class="author">
			af <?php print theme('username', $node); ?>
		</span>	



		<?php if (count($taxonomy)){ ?>
		  <div class="taxanomy">
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