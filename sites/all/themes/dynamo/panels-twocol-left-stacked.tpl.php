<!--panels-twocol-left-stacked.tpl.php-->
<div class="panel-twocol-left-stacked clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

	<div class="content-left">

		<div class="content-top">
			<?php print $content['top']; ?>
		</div>
		
		<div class="panel-left">
			<?php print $content['left']; ?>      
		</div>


		<?php if (!empty($content['bottom'])): ?>
	  <div class="panel-bottom">
	    <?php print $content['bottom']; ?>  
	  </div>
		<?php endif; ?>	


	</div>

	<div class="content-right">
		<?php print $content['right']; ?>
	</div>  



</div>  
<!--/panels-twocol-left-stacked.tpl.php-->