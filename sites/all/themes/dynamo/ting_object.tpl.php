<?php
// $Id$
/**
 * @file ting_object.tpl.php
 *
 * Template to render objects from the Ting database.
 *
 * Available variables:
 * - $object: The TingClientObject instance we're rendering.
 */
?>
<!-- ting_object.tpl -->
<div id="ting-object">

  <div class="content-left">

    <div class="tab-navigation">

      <ul>
        <li class="active"><a href="#">Materialer</a></li>
      </ul>

    </div>

    <div class="tab-navigation-main">
      <div class="tab-navigation-main-inner">
        <div id="ting-item-<?php print $object->localId; ?>" class="ting-item ting-item-full">
          <div class="ting-overview clearfix">

            <div class="left-column left">
              <div class="picture">

                <?php $image_url = ting_covers_object_url($object, '180_x'); ?>
                <?php if ($image_url) { ?>
                  <?php print theme('image', $image_url, '', '', null, false); ?>
                <?php } ?>
              </div>

            </div>

            <div class="right-column left">
              <h2><?php print check_plain($object->record['dc:title'][''][0]); ?></h2>
              <?php foreach (array_diff(array_keys($object->record['dc:title']), array('')) as $type) { ?>
                <?php foreach ($object->record['dc:title'][$type] as $title) { ?>
                  <h2><?php print check_plain($title); ?></h2>
                <?php } ?>
              <?php } ?>
              <?php if (!empty($object->record['dcterms:alternative'][''])) { ?>
                <?php foreach ($object->record['dcterms:alternative'][''] as $title) { ?>
                  <h2>(<?php print check_plain($title); ?>)</h2>
                <?php } ?>
              <?php } ?>
              <div class='creator'>
                <span class='byline'><?php echo ucfirst(t('by')); ?></span>
                <?php
                  foreach ($object->creators as $i => $creator) {
                    if ($i) {
                      print ', ';
                    }
                    print l($creator, 'search/ting/' . $creator, array('attributes' => array('class' => 'author')));
                  }
                ?>
                <?php if (!empty($object->date)) { ?>
                  <span class='date'>(<?php print $object->date; ?>)</span>
                <?php } ?>
              </div>
              <p><?php print check_plain($object->record['dcterms:abstract'][''][0]); ?></p>
              <?php if ($object->type != 'Netdokument') { ?>
                <div class="alma-status waiting"><?php print t('waiting for data'); ?></div>
              <?php } ?>
            </div>

            <?php print theme('alma_cart_reservation_buttons', $object); ?>

          </div>

          <div class="object-information clearfix">
            <?php 
            //we printed the first part up above so remove that 
            unset($object->record['dcterms:abstract'][''][0]);
            ?>
            <div class="abstract"><?php print implode(' ; ', format_danmarc2((array)$object->record['dcterms:abstract'][''])) ?></div>

            <?php print theme('item_list',$object->record['dc:type']['dkdcplus:BibDK-Type'], t('Type'), 'span', array('class' => 'type'));?>
            <?php print theme('item_list',$object->record['dc:format'][''], t('Format'), 'span', array('class' => 'format'));?>
            <?php print theme('item_list',$object->record['dc:source'][''], t('Original title'), 'span', array('class' => 'format'));?>
            <?php print theme('item_list',$object->record['dc:identifier']['dkdcplus:ISBN'], t('ISBN no.'), 'span', array('class' => 'identifier'));?>
            <?php print theme('item_list',array(l($object->record['dc:identifier']['dcterms:URI'][0], $object->record['dc:identifier/dcterms:URI'][0])), t('Host publication'), 'span', array('class' => 'identifier'));?>
            <?php print theme('item_list',$object->record['dc:language'][''], t('Language'), 'span', array('class' => 'language'));?>
            <?php print theme('item_list',$object->record['dc:language']['oss:spoken'], t('Speech'), 'span', array('class' => 'language'));?>
            <?php print theme('item_list',$object->record['dc:language']['oss:subtitles'], t('Subtitles'), 'span', array('class' => 'language'));?>
            <?php print theme('item_list',$object->record['dc:subject']['dkdcplus:DBCS'], t('Subject'), 'span', array('class' => 'subject'));?>
            <?php print theme('item_list',$object->record['dc:publisher'][''], t('Publisher'), 'span', array('class' => 'publisher'));?>
            <?php print theme('item_list',$object->record['dc:rights'][''], t('Rettigheder'), 'span', array('class' => 'language'));?>
            <?php // print theme('item_list',$object->data->relation, t('Relation'), 'span', array('class' => 'relation'));?>
            <?php // print theme('item_list',$object->data->coverage, t('Coverage'), 'span', array('class' => 'coverage'));?>
            <?php // print theme('item_list',$object->data->rights, t('Rights'), 'span', array('class' => 'rights'));?>
          </div>

          <?php
          $collection = ting_get_collection_by_id($object->id);
          if ($collection instanceof TingClientObjectCollection && is_array($collection->types)) {
            // Do we have more than only this one type?
            if (count($collection->types) > 1) {
              print '<div class="ding-box-wide object-otherversions">';
              print '<h3>'. t('Also available as: ') . '</h3>';  
              print "<ul>";
              foreach ($collection->types as $type) {
                if ($type != $object->type) {
                  $material_links[] = '<li class="category">' . l($type, $collection->url). '</li>';
                }
              }
              print implode(' ', $material_links);
              print "</ul>";
              print "</div>";
            }
          }
          ?>
          

          <?php
          $referenced_nodes = ting_reference_nodes($object);
          if ($referenced_nodes) {
            print '<h3>Omtale på websitet</h3>';
            foreach ($referenced_nodes as $node) {
              print node_view($node, TRUE);
            }
          }
          ?>

          <?php if ($object->type[0] != 'Netdokument') { ?>
            <div class="ding-box-wide alma-availability">
              <h3>Følgende biblioteker har "<?php print check_plain($object->title); ?>" hjemme:</h3>
              <ul class="library-list">
                <li class="alma-status waiting even"><?php print t('waiting for data'); ?></li>
              </ul>
            </div>
          <?php } ?>
        </div>
      </div>
    </div>
  </div>

</div>

