<div class="facet-browser">
	<?php echo theme('ting_facet_browser', $result); ?>
</div>
<div class="search-results">
	<ol>
		<?php foreach ($result->records as $record) : ?>
			<li>
				<?php echo theme('ting_record', $record); ?>
			</li>
		<?php endforeach; ?>
	</ol>
</div>