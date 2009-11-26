 <!-- views-view-fields  event-list  page.tpl.php  (panel pane)-->
<?php 

  // Convert the date value to timestamp
  $date = strtotime($fields['field_datetime_value']->raw . 'Z');
?>
<div class="node-teaser clearfix">

  <div class="picture">

    <?php print $fields['field_list_image_fid']->content; ?>
  </div>

  <div class="info">
  
		<h2><?php print $fields['title']->content; ?></h2>

		<div class="meta">
		  <?php print format_date($date, 'custom', 'l j M h:i'); ?>
		  <span class="libary-tag"><?php print $fields['field_library_ref_nid']->content; ?></span>, 
		  <span class="price"><?php print $fields['field_entry_price_value']->content; ?></span>
		</div>    
    
		<p> 
		 <?php 
	 			if($fields['field_teaser_value']->content){
					print $fields['field_teaser_value']->content; 

				}else{
					print $fields['body']->content;       
	 			}
 			?>
		</p> 
		
    <?php print $fields['edit_node']->content; ?>  
  </div>
</div>

