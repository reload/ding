<?php
// $Id$

/**
 * @file ding_event_similar_events.tpl.php
 * Template to display similar events, usually in the context of a Panel.
 */
?>


<ul id="event-similar" class="jcarousel-skin-events">
<?php foreach ($events as $node): ?>
  <li>
    <h4><?php print l($node->title, 'node/' . $node->nid); ?></h4>

    <?php if($node->field_image['0']['filepath']){ ?>
        <?php print theme('imagecache', '120_120', $node->field_image['0']['filepath']); ?>      
    <?php } ?>

    <?php 
		  $date = strtotime($node->field_datetime[0]['value']);
		  $date2 = strtotime($node->field_datetime[0]['value2']);    

		  print format_date($date, 'custom', "j F Y") ."<br/>";
		  print format_date($date, 'custom', "H:i") ." - ";
		  print format_date($date2, 'custom', "H:i");		  
		 ?> 
		 <br/>
		  pris: <?php print $node->field_entry_price[0]['value']; ?>
  
			<?php print $node->teaser; ?> 

  </li>
<?php endforeach; ?>
</ul>

