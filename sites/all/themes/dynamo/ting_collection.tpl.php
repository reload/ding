<?php
// $Id$
/**
 * @file ting_object.tpl.php
 *
 * Template to render a collection objects from the Ting database.
 *
 * Available variables:
 * - $collection: The TingClientObjectCollection instance we're rendering.
 * - $sorted_collection: Array of TingClientObject instances sorted by type.
 * - $common_object: TingClientObject holding information for the entire collection.
 */
?>
<!--ting-collection-->
<div id="ting-collection">
	<div class="content-left">

		<div class="tab-navigation">

			<ul>
				<li class="active"><a href="#">Materialer</a></li>
			</ul>

		</div>

		<div class="tab-navigation-main">
			<div class="tab-navigation-main-inner">
				<?php
				// material types retrieved from preprocess hook...
				// $common_object is fetched from the preprocess hook
				?>
				
				<div class="ting-overview clearfix">
			    	<?php 
					// 	TODO set false to true ?
					//print theme('image', $tingClientObject->additionalInformation->detailUrl, '', '', null, false);
		 			?>
		 			
	   				<h1><?php print $common_object->data->title['0'];?></h1>
	
					<div class='creator'>
						<span class='byline'><?php echo ucfirst(t('by')); ?></span>
            <?php
            foreach ($common_object->data->creator as $i => $creator) {
              if ($i) {
                print ', ';
              }
              print l($creator, 'search/ting/' . $creator, array('attributes' => array('class' => 'author')));
            }
            ?>
						<span class='date'>(<?php echo $common_object->data->date[0]; ?>)</span> 
					</div>		 			

					<p><?php print $common_object->data->abstract[0];?></p>

					<div class='terms'>
						<?php //print theme('item_list', $tingClientObject->data->subject, t('Terms:'), 'span', array('class' => 'subject'));?>	
						<span class='title'><?php echo t('Terms:'); ?></span>
						<?php
						foreach ($common_object->data->subject as $term) {
							if(!empty($term)) {
								$terms[] = "<span class='term'>".l($term, 'search/ting/'.$term)."</span>";
							}
						}
						echo implode(", ", $terms);
						?>
					</div>
					
          <?php 
            /*only one collection ? dont print it*/
            if(count($sorted_collection) > 1){ 
            ?>
  					<div class='material-links'>
  						<span class='title'><?php echo t('Go to material:'); ?></span>
  						<?php
  						foreach ($sorted_collection as $category => $objects) {
  							$material_links[] = '<span class="link"><a href="#'.$category.'">'.t($category).'</a></span>';
  						}
  						echo implode(", ", $material_links);
  						?>
					  </div>
					<?php } ?>

				</div>


				<?php
 				foreach ($sorted_collection as $category => $objects) {		
          if(count($sorted_collection) > 1){ 
					  print '<h2>'.$category.'<a name="'.$category.'"></a></h2>';
 					}

					foreach ($objects as $tingClientObject) {
				    // now display all the materials
				?>

				<div id="ting-item-<?php print $tingClientObject->data->localId; ?>" class="ting-item clearfix">

          <div class="content clearfix">
  		  		<div class="picture">
  						<?php $image_url = ting_covers_object_url($tingClientObject, '80_x'); ?>
  						<?php if ($image_url) { ?>
  							<?php print theme('image', $image_url, '', '', null, false); ?>
  						<?php } ?>
  					</div>

  				  <div class="info">
				  		<span class='date'><?php echo $tingClientObject->data->date[0]; ?></span> 
  						<h3><?php print l($tingClientObject->data->title['0'], $tingClientObject->url, array("attributes"=>array('class' => 'alternative'))); ?></h3>


  						<em><?php echo t('by'); ?></em>
  						<?php echo l($tingClientObject->data->creator[0], 'search/ting/'.$tingClientObject->data->creator[0], array("attributes"=>array('class' => 'author alternative'))); ?>
						
  						<div class='language'><?php echo t('Language') . ': ' . $tingClientObject->data->language[1]; ?></div>
  						<?php
  						for ($i = 1; $i < count($tingClientObject->data->creator); $i++) {
  							if($extradesc = $tingClientObject->data->creator[$i]) { print "<p>".$extradesc."</p>"; }
  						}
  						?>

  						<div class="more">
  						  <?php print l(t('More information'), $tingClientObject->url, array('attributes' => array('class' => 'more-link')) ); ?>  
  						</div>
            
              <div class="alma-status waiting">Afventer data…</div>

  					</div>

          </div>

					<div class="cart">
            <?php print theme('alma_cart_reservation_buttons', $tingClientObject); ?>
					</div>

				</div>

				<?php 
					} // foreach objects
				} //foreach collection
				?>

        <?php
        $referenced_nodes = ting_reference_nodes($collection);
        if ($referenced_nodes) {
          print '<h3>Omtale på websitet</h3>';
          foreach ($referenced_nodes as $node) {
            print node_view($node, TRUE);
          }
        }
        ?>
			</div>	
		</div>
	</div>

</div>

<!--/ting-collection-->

