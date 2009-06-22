<?php 
  //converts the date value to time
  $date = strtotime($fields['field_datetime_value']->raw);

  if( date('d-m-Y') == date("d-m-Y", $date) ){
    $is_today = "today";
  }
?>


<div class="leaf <?php print $is_today ?>">
  <div class="day"><?php print format_date($date, 'custom', 'l');?></div>
  <div class="date"><?php print date("j", $date);?></div>
  <div class="month"><?php print format_date($date, 'custom', 'M');?></div>
</div>

<div class="info">
  <?php print $fields['field_library_ref_nid']->content; ?>
  <h4><?php print $fields['title']->content; ?></h4>
  <span class="time"><?php print date("h:i", $date); ?>  

    <?php if($fields['field_entry_price_value']->content){ ?>
      <?php print $fields['field_entry_price_value']->content; ?>
    <?php }else{ ?>
      Gratis
    <?php }  ?>
    
  </span>
</div>  