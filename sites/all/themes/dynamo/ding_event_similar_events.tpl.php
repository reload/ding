<?php
// $Id$

/**
 * @file ding_event_similar_events.tpl.php
 * Template to display similar events, usually in the context of a Panel.
 */
?>

<h4 class="title"><?php print t('Other events you may like'); ?></h4>
<ul id="event-similar" class="jcarousel-skin-events">
<?php foreach ($events as $node): ?>
  <li>
    <h4><?php print l($node->title, 'node/' . $node->nid); ?></h4>

    	<?php 
		  $date = strtotime($node->field_datetime[0]['value']);
		  $date2 = strtotime($node->field_datetime[0]['value2']);    

		  if(date("Ymd", $date) == date("Ymd", $date2))
		  {
		  	print format_date($date, 'custom', "j. F Y");
		  }
		  elseif(date("Ym", $date) == date("Ym", $date2))
		  {
		  	print format_date($date, 'custom', "j.") . "-" . format_date($date2, 'custom', "j. F Y");
		  }
		  else
		  {
		  	print format_date($date, 'custom', "j. M.") . " - " . format_date($date2, 'custom', "j. M. Y");
		  }
		  
		//  print format_date($date, 'custom', "H:i") ." - ";
		//  print format_date($date2, 'custom', "H:i");		  
		 ?> 

    <?php if($node->field_image['0']['filepath']){ ?>
        <?php print theme('imagecache', '180_x', $node->field_image['0']['filepath']); ?>      
    <?php } 
    	  else
    	  {
    	  	print $node->teaser;
    	  }
    
    ?>

		 <?php //print t('pris:') . $node->field_entry_price[0]['value']; ?>
  
		<?php //print $node->teaser; ?> 

  </li>
<?php endforeach; ?>
</ul>

