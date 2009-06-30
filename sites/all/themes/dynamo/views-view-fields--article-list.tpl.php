<?php
  dsm($fields);
?>

<div class="subject"><?php print $fields['tid']->content; ?> </div>
<h2><?php print $fields['title']->content; ?></h2>
<div class="meta">
  <?php if($fields['field_library_ref_nid']->content){ ?>
  <ul>
    <li><?php print $fields['field_library_ref_nid']->content; ?></li>
  </ul>
  <?php } ?>
  
  <?php print $fields['created']->content; ?>
  <i><?php print t('by'); ?></i>
  
  <span class="author">
    <?php print $fields['name']->content; ?>  
  </span>
  <?php 
    if($fields['comment_count']->raw >= "1"){
      print "(". $fields['comment_count']->content .")";
    }  
  ?>
</div>

<p>
  <?php print $fields['field_teaser_value']->content; ?>  
  
  <?php print $fields['body']->content; ?>    
  <?php print $fields['edit_node']->content; ?>
  <span class="more-link"><?php print $fields['view_node']->content; ?></span>

</p>