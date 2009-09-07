<?php 
//	dsm($object);
?>
<div id="ting-object">

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

				<div class="ting-overview clearfix">

					<div class="left-column left">
			  		<div class="picture">

			  			<?php $image_url = ting_search_cover_url($object, '180_x'); ?>
			  			<?php if ($image_url) { ?>
			  				<?php print theme('image', $image_url, '', '', null, false); ?>
			  			<?php } ?>
						</div>

					</div>

					<div class="right-column left">
						<h2><?php print $object->data->title[0];?></h2>						
					<div class='creator'>
						<span class='byline'><?php echo ucfirst(t('by')); ?></span>
						<?php echo l($object->data->creator[0], 'search/ting/'.$object->data->creator[0], array("attributes"=>array('class' => 'author'))); ?>
						<span class='date'>(<?php echo $object->data->date[0]; ?>)</span>
					</div>
					<p><?php print $object->data->description[0];?></p>
					</div>

					<div class="buttons">
						<div class="button button-orange"><a href="">Reserver nu</a></div>
						<div class="button button-green"><a href="">Læg i kurv</a></div>
					</div>

					
				</div>

				<div class="object-information clearfix">
					<?php 
					//we printed the first part up above so remove that 
					unset($object->data->description[0]);
					?>
					<div class="description"><?php print implode(' ; ', format_danmarc2($object->data->description)) ?></div>

					<?php print theme('item_list',$object->data->identifier, t('Identifier'), 'span', array('class' => 'identifier'));?>	
					<?php print theme('item_list',$object->data->subject, t('Subject'), 'span', array('class' => 'subject'));?>	
					<?php print theme('item_list',$object->data->publisher, t('Publisher'), 'span', array('class' => 'publisher'));?>						
					<?php print theme('item_list',$object->data->type, t('Type'), 'span', array('class' => 'type'));?>											
					<?php print theme('item_list',$object->data->format, t('Format'), 'span', array('class' => 'format'));?>											
					<?php print theme('item_list',$object->data->language, t('Language'), 'span', array('class' => 'language'));?>						
					<?php print theme('item_list',$object->data->relation, t('Relation'), 'span', array('class' => 'relation'));?>											
					<?php print theme('item_list',$object->data->coverage, t('Coverage'), 'span', array('class' => 'coverage'));?>											
					<?php print theme('item_list',$object->data->rights, t('Rights'), 'span', array('class' => 'rights'));?>						
				</div>

				<div class="ding-box-wide">
					<h3>Følgende biblioteker har <?php print $object->data->title[0];?> hjemme:</h3>
						<ul>
							<li class="even"><?php print l('hovedbibloteket', 'node/'.$node->nid);?></li>
							<li class="odd"><?php print l('hovedbibloteket', 'node/'.$node->nid);?></li>
							<li class="even"><?php print l('hovedbibloteket', 'node/'.$node->nid);?></li>
						</ul>
				</div>


				<div class="object-otherversions">
					se som xxx .... yyy
				</div>
				
        <?php
        $referenced_nodes = ting_reference_nodes($object);
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

	<div class="content-right">
	 KAmpagner
	</div>
	
</div>
