<a href="<?php echo $object->url ?>" title="<?php echo implode($object->data->title, ', ') ?>: <?php if (!empty($object->data->creator)) { echo implode($object->data->creator, ', '); } ?>">
  <span class="title"><?php echo implode($object->data->title, ', ') ?></span>
  <?php if (!empty($object->data->creator)) { ?>
    <span class="creator"><?php echo implode($object->data->creator, ', ') ?></span>
  <?php } ?>
</a>
