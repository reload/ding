<div class="facet-browser">
	<?php echo theme('ding_ting_facet_browser', $result); ?>
</div>
<div class="search-results">
	<ol>
		<?php foreach ($result->records as $record) : ?>
			<li>
				<?php echo theme('ding_ting_search_record', $record); ?>
			</li>
		<?php endforeach; ?>
	</ol>
</div>