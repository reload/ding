<?php
// $Id$

/**
 * @file
 * Template to render a Ting collection of books.
 */
?>
  <li>

    <div class="picture">
      <?php $image_url = ting_covers_collection_url($collection->objects[0], '80_x'); ?>
      <?php if ($image_url) { ?>
        <?php print l(theme('image', $image_url, '', '', null, false), $collection->url, array('html' => true)); ?>
      <?php } ?>
    </div>

    <div class="record">
      <div class="types">
        <?php print $type_list; ?>
      </div>

      <h3>
        <?php print l(implode(', ', $collection->objects[0]->data->title), $collection->url, $options= array('attributes' => array('class' =>'title')) );?> 
      </h3>

      <div class="meta">
        <?php if ($collection->objects[0]->data->creator) : ?>
          <span class="creator">
            <?php echo t('By %creator_name%', array('%creator_name%' => implode(', ', $collection->objects[0]->data->creator))) ?>
          </span>
        <?php endif; ?>
        <?php if ($collection->objects[0]->data->date) : ?>
          <span class="publication_date">
            <?php echo t('(%publication_date%)', array('%publication_date%' => implode(', ', $collection->objects[0]->data->date))) /* TODO: Improve date handling, localizations etc. */ ?>
          </span>
        <?php endif; ?>
      </div>

      <?php if ($collection->objects[0]->data->abstract) : ?>
      <div class="abstract">
        <p>
          <?php echo $collection->objects[0]->data->abstract[0] ?>
        </p>
      </div>
      <?php endif; ?>

      <?php if ($collection->objects[0]->data->subject) : ?>
        <div class="subjects">
          <h4><?php echo t('Subjects:') ?></h4>
          <ul>
            <?php foreach ($collection->objects[0]->data->subject as $subject) : ?>
              <li><?php echo $subject ?></li>
            <?php endforeach; ?>
          </ul>
        </div>
      <?php endif; ?>

    </div>

  </li>

