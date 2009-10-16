<?php if ($objects && (sizeof($objects) > 0)) :?>
  <div class="ting-recommendation-list">
    <div class="ding-box-wide">
		  <ul>
		  <?php foreach ($objects as $i => $object) : ?>
		    <li class="<?php echo (($i % 2) == 0) ? 'odd' : 'even' ?> <?php echo ((is_int(($i-1) / 4)) || (is_int($i / 4))) ? 'either' : 'or' ; ?>">
		      <?php echo theme('ting_recommendation_panes_recommendation_list_entry', $object); ?>      
		    </li>    
		  <?php endforeach; ?>
		  </ul>
		</div>
	</div>
<?php endif; ?>