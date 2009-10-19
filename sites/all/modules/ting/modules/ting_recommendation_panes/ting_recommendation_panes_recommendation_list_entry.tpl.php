<a href="<?php echo $object->url ?>" title="<?php echo implode($object->data->title, ', ') ?>: <?php echo implode($object->data->creator, ', ') ?>">
  <span class="title"><?php echo implode($object->data->title, ', ') ?></span>
  <span class="creator"><?php echo implode($object->data->creator, ', ') ?></span>
</a>