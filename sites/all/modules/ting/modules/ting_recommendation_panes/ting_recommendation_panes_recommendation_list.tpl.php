<?php if ($objects && (sizeof($objects) > 0)) :?>
  <div class="ting-recommendation-list">
	  <ul>
	  <?php foreach ($objects as $i => $object) : ?>
	    <li class="<?php echo (($i % 2) == 0) ? 'odd' : 'even' ?>">
	      <?php echo theme('ting_recommendation_panes_recommendation_list_entry', $object); ?>      
	    </li>    
	  <?php endforeach; ?>
	  </ul>
	</div>
<?php endif; ?>