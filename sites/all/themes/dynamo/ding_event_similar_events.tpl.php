<?php
// $Id$

/**
 * @file ding_event_similar_events.tpl.php
 * Template to display similar events, usually in the context of a Panel.
 */
?>
<ul class="similar-events jcarousel-skin-biblo">
<?php foreach ($events as $node): ?>
  <li>
    <?php // dsm($node); ?>


  <?php if($node->field_image_default['0']['filepath']){ ?>
    <div class="picture">
      <?php print theme('imagecache', '50_50_crop', $node->field_image_default['0']['filepath']); ?>      
    </div>
  <?php } ?>

    <?php print l($node->title, 'node/' . $node->nid); ?>
    <p>
    <?php print $node->field_entry_price['0']['value']; ?>
    <?php print $node->field_datetime['0']['value']; ?>    
    <?php print $node->field_datetime['0']['value2']; ?>    
  </p>
    <p>
    <?php print truncate_utf8($node->teaser, 150, $wordsafe = FALSE, $dots = TRUE); ?>
    </p>




 

  
  </li>
  

<?php endforeach; ?>
</ul>

