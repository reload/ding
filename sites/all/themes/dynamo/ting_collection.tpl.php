<?php 
?>
<div id="ting-collection">
	<div class="content-left">

		<div class="tab-navigation">

			<ul>
				<li class="active"><a href="#">Materialer</a></li>
				<li><a href="#">Forfatterportræt </a></li>
				<li  class="active"><a href="#">Anmeldelser </a></li>				
				<li class="active"><a href="#">Materialer</a></li>
				<li><a href="#">Forfatterportræt </a></li>
				<li  class="active"><a href="#">Anmeldelser </a></li>				
				<li class="active"><a href="#">Materialer</a></li>
				<li><a href="#">Forfatterportræt </a></li>
				<li  class="active"><a href="#">Anmeldelser </a></li>				
			</ul>

		</div>

		<div class="tab-navigation-main">
			<div class="tab-navigation-main-inner">
				<?php
				// material types retrieved from preprocess hook...
				
				// show the common descriptor
				$preferred_typekey = "Bog";
				$preferred_language = "dan";
					
				// Try to fetch a danish book
				$common_object = null;
				if(array_key_exists($preferred_typekey, $sorted_collection)) {
					foreach($sorted_collection[$preferred_typekey] as $object)
					{	
						if($object->data->language[0] == $preferred_language)
						{
							if(!empty($object->data->description[0])) {
								$common_object = $object;
								break;
							}			
						}
					}
				}
				
				// just get a material with a description
				if(!$common_object) {
					foreach($sorted_collection as $typekey => $objects)
					{
						$common_object = $objects[0];
						
						if(empty($common_object->data->description[0])) {
							continue;
						}
						break;
					}
				}
				?>
				
				<div class="ting-overview clearfix">
			    	<?php 
					// 	TODO set false to true ?
					//print theme('image', $tingClientObject->additionalInformation->detailUrl, '', '', null, false);
		 			?>
		 			
	   				<h2><?php print $common_object->data->title['0'];?></h2>
	
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

					<p><?php print $common_object->data->description[0];?></p>

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
					
					<div class='material-links'>
						<span class='title'><?php echo t('Go to material:'); ?></span>
						<?php
						foreach ($sorted_collection as $category => $objects) {
							$material_links[] = '<span class="link"><a href="#'.$category.'">'.t($category).'</a></span>';
						}
						echo implode(", ", $material_links);
						?>
					</div>
					
				</div>

								


				<?php
 				foreach ($sorted_collection as $category => $objects) {		
					print '<h3>'.$category.'<a name="'.$category.'">&nbsp;</a></h3>';
 					
					foreach ($objects as $tingClientObject) {
	
				// now display all the materials
				?>

				<div class="collection clearfix">

		  		<div class="picture">
						<?php $image_url = ting_search_cover_url($tingClientObject, '80_x'); ?>
						<?php if ($image_url) { ?>
							<?php print theme('image', $image_url, '', '', null, false); ?>
						<?php } ?>
					</div>

				  <div class="content">
				  		<span class='date'><?php echo $tingClientObject->data->date[0]; ?></span> 
						<h5><?php print l($tingClientObject->data->title['0'], $tingClientObject->url, array("attributes"=>array('class' => 'alternative'))); ?></h5>
						<span class='byline'><?php echo t('by'); ?></span>
						<?php echo l($tingClientObject->data->creator[0], 'search/ting/'.$tingClientObject->data->creator[0], array("attributes"=>array('class' => 'author alternative'))); ?>		
						
						<div class='language'><?php echo t('Language: ') . $tingClientObject->data->language[1]; ?></div>
						<?php
						for ($i=1; $i<count($tingClientObject->data->creator); $i++)
						{
							if($extradesc = $tingClientObject->data->creator[$i]) { print "<p>".$extradesc."</p>"; }
						}

						for ($i=1; $i<count($tingClientObject->data->description); $i++)
						{
							if($extradesc = $tingClientObject->data->description[$i]) { print "<p>".$extradesc."</p>"; }
						}
						?>
		
						
						
					<?php
				//	echo "<pre>";
				//	print_r($collection);
				//	echo "</pre>"; ?>
						
					<?php //print theme('item_list', format_danmarc2($tingClientObject->data->description), t('Description'), 'span', array('class' => 'description'));?>

						<?php //print theme('item_list', $tingClientObject->data->subject, t('Terms'), 'span', array('class' => 'subject'));?>
						<?php //print theme('item_list', $tingClientObject->data->type, t('Type'), 'span', array('class' => 'type'));?>
						<?php //print theme('item_list', $tingClientObject->data->format, t('Format'), 'span', array('class' => 'format'));?>
						<?php //print theme('item_list', $tingClientObject->data->source, t('Source'), 'span', array('class' => 'source'));?>
						<?php //print theme('item_list', $tingClientObject->data->publisher, t('Publisher'), 'span', array('class' => 'publisher'));?>
						<?php //print theme('item_list', $tingClientObject->data->language, t('Language'), 'span', array('class' => 'language'));?>

						<?php print l(t('More information'), $tingClientObject->url, array('attributes' => array('class' => 'more-link')) ); ?>

						<ul class="types">
							<li class="out">UDLÅNT</li>
							<li class="reserved">UDLÅNT - x-y ugers ventetid</li>					
							<li class="available">HJEMME</li>										
						</ul>

					</div>
					<div class="cart">

						<ul>
							<li class="button button-orange"><a href="">Reserver nu</a></li>
							<li class="button button-green"><a href="">Læg i kurv</a></li>
						</ul>

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
