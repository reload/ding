<?php 
//dsm($collection);
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
				// retrieve material types...
				$materialtypes = array();
				foreach ($collection->objects as $key => $value) {
					$type = $collection->objects[$key]->data->type['0'];
					$lang = $collection->objects[$key]->data->language['0'];
					$type_key = $type .'_'.$lang;
					
					if (!in_array($type_key, $materialtypes)) {
						$materialtypes[] = $type_key;
					}
				}
				?>


				<?php
				foreach ($collection->objects as $key => $value) {
				//	dsm($key);
				?>
				
				<?php if($key == "0"){ ?>


					<div class="ting-overview clearfix">
				    	<?php 
						// 	TODO set false to true ?
						//print theme('image', $collection->objects[$key]->additionalInformation->detailUrl, '', '', null, false);
			 			?>

						<div class="left-column left">
				  		<div class="picture">
				  			<?php $image_url = ting_search_cover_url($collection->objects[$key], '180_x'); ?>
				  		 	<?php if ($image_url) { ?>
				  				<?php print theme('image', $image_url, '', '', null, false); ?>
				  			<?php } ?>
							</div>


						</div>
						<p><?php print $collection->objects[$key]->data->description[0];?></p>

						<div class='terms'>
							<?php //print theme('item_list', $collection->objects[$key]->data->subject, t('Terms:'), 'span', array('class' => 'subject'));?>	
							<span class='title'><?php echo t('Terms:'); ?></span>
							<?php
							foreach ($collection->objects[$key]->data->subject as $term) {
								$terms[] = "<span class='term'>".l($term, 'search/ting/'.$term)."</span>";
							}
							echo implode(", ", $terms);
							?>
						</div>
						
						<div class='material-links'>
							<span class='title'><?php echo t('Go to materials:'); ?></span>
							<?php
							foreach ($materialtypes as $materialtype) {
								$material_links[] = '<span class="link"><a href="#'.$materialtype.'">'.t($materialtype).'</a></span>';
							}
							echo implode(", ", $material_links);
							?>
						</div>
						
					</div>

					<hr />

				<?php }
				// now display all the materials
				?>

				<div class="collection clearfix">

		  		<div class="picture">
						<?php $image_url = ting_search_cover_url($collection->objects[$key], '80_x'); ?>
						<?php if ($image_url) { ?>
							<?php print theme('image', $image_url, '', '', null, false); ?>
						<?php } ?>
					</div>

				  <div class="content">
				  		<span class='date'><?php echo $collection->objects[$key]->data->date[0]; ?></span> 
						<h5><?php print l($collection->objects[$key]->data->title['0'], $collection->objects[$key]->url, array("attributes"=>array('class' => 'alternative'))); ?></h5>
						<span class='byline'><?php echo t('by'); ?></span>
						<?php echo l($collection->objects[$key]->data->creator[0], 'search/ting/'.$collection->objects[$key]->data->creator[0], array("attributes"=>array('class' => 'author alternative'))); ?>		
						<? if($extradesc = $collection->objects[$key]->data->description[1]) { print "<p>".$extradesc."</p>"; } ?>
						
						<p>På dansk ved... (mangler data)</p>
						<p>Illustreret af... (mangler data)</p>
						
					<?php //print theme('item_list', format_danmarc2($collection->objects[$key]->data->description), t('Description'), 'span', array('class' => 'description'));?>

						<?php //print theme('item_list', $collection->objects[$key]->data->subject, t('Terms'), 'span', array('class' => 'subject'));?>
						<?php //print theme('item_list', $collection->objects[$key]->data->type, t('Type'), 'span', array('class' => 'type'));?>
						<?php //print theme('item_list', $collection->objects[$key]->data->format, t('Format'), 'span', array('class' => 'format'));?>
						<?php //print theme('item_list', $collection->objects[$key]->data->source, t('Source'), 'span', array('class' => 'source'));?>
						<?php //print theme('item_list', $collection->objects[$key]->data->publisher, t('Publisher'), 'span', array('class' => 'publisher'));?>
						<?php //print theme('item_list', $collection->objects[$key]->data->language, t('Language'), 'span', array('class' => 'language'));?>

						<?php print l(t('More information'), $collection->objects[$key]->url, array('attributes' => array('class' => 'more-link')) ); ?>

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
				
				} //foreach collection
				?>

			
			</div>	
		</div>
	</div>


	
</div>
