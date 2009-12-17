<?php
// $Id$
/**
 * @file ting_object_title.tpl.php
 *
 * Template to render a Ting object title (including author etc.).
 *
 * Available variables:
 * - $object: The TingClientObject instance we're rendering.
 */
?>
<!-- ting_object_title.tpl -->
<?php if (!$object) { ?>
  <?php print t('Title not available.'); ?>
<?php } else { ?>
  <?php if ($display_image) { ?>

    <?php $image_url = ting_covers_object_url($object, '80_x'); ?>
    <?php if ($image_url) { ?>
      <?php print '<span class="image">' . theme('image', $image_url, '', '', NULL, FALSE) .'</span>'; ?>
    <?php } ?>
  <?php } ?>
  <span class="title"><?php print l($object->data->title[0], $object->url); ?></span>
  <span class='creator'>
    <span class='byline'><?php echo t('by'); ?></span>
    <?php
    foreach ($object->data->creator as $i => $creator) {
      if ($i) {
        print ', ';
      }
      print check_plain($creator);
    }
    ?>
    <span class='date'>(<?php echo $object->data->type[0]; ?>)</span>

<?php } ?>
<!-- /ting_object_title.tpl -->
