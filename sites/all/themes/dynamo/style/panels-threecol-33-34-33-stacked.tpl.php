<!--panels-threecol-33-34-33-stacked.tpl.php-->

<div class="panel-threecol-33-34-33-stacked clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>	

	<div class="panel-top">
 
	  <ul id="frontpagecarousel" class="jcarousel-skin-biblo">
	    <li><img src="<?php print path_to_theme();?>/images/test-carousel-1.jpg" width="895" height="183" alt="Test Carousel 1" /></li>
	    <li><img src="<?php print path_to_theme();?>/images/test-carousel-2.jpg" width="895" height="183" alt="Test Carousel 1" /></li>
	    <li><img src="<?php print path_to_theme();?>/images/test-carousel-3.jpg" width="895" height="183" alt="Test Carousel 1" /></li>
	    <li><img src="<?php print path_to_theme();?>/images/test-carousel-4.jpg" width="895" height="183" alt="Test Carousel 1" /></li>    
	  </ul>

	  <ul id="frontpagecarousel-controller">
	    <li class="active">Alle</li>
	    <li><a href="#">Romaner</a></li>
	    <li><a href="#">Fagbøger</a></li>
	    <li><a href="#">Musik</a></li>
	    <li><a href="#">Film</a></li>
	  </ul>

	</div>

	<?php if (!empty($content['top'])): ?>
  <div class="panel-top">
    <?php print $content['top']; ?>  
  </div>
	<?php endif; ?>

	<div class="panel-content">
  
	  <div class="panel-left">
	    <?php print $content['left']; ?>  
	  </div>
	  <div class="panel-middle">
	    <?php print $content['middle']; ?>  
	  </div>

	  <div class="panel-right">
	    <?php print $content['right']; ?>  
	  </div>

	</div>

	<?php if (!empty($content['bottom'])): ?>
  <div class="panel-bottom">
    <?php print $content['bottom']; ?>  
  </div>
	<?php endif; ?>	


</div>
<!-- / panels-threecol-33-34-33-stacked.tpl.php-->