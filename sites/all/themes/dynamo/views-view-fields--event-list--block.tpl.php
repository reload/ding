<!-- views-view-fields  event-list  BLOCK.tpl.php-->
<?php 
  //converts the date value to time
  $date = strtotime($fields['field_datetime_value']->raw . 'Z');
  if( date('d-m-Y') == format_date($date,'custom', "d-m-Y") ){
    $is_today = "today";
  }

	/* Price */
	if($fields['field_entry_price_value']->content > 0){
		$price = check_plain($fields['field_entry_price_value']->raw) ." ". t('Kr');
  	//print $fields['field_entry_price_value']->content;
	}else{ 
  	$price = t('Free');
	}  
?>
<div class="calendar-leafs">

  <div class="leaf <?php print $is_today ?>">
    <div class="day"><?php print format_date($date, 'custom', 'l');?></div>
    <div class="date"><?php print format_date($date, 'custom', 'j');?></div>
    <div class="month"><?php print format_date($date, 'custom', 'M');?></div>
  </div>

  <div class="info">
    <?php print $fields['field_library_ref_nid']->content; ?>
    <h4><?php print $fields['title']->content; ?></h4>
    <span class="time">
			<?php print format_date($date, 'custom', 'H:i');?>
      <?php print $price; ?>
    </span>
  </div>

</div>  