<li>
	<a href="<?php echo $collection->url ?>">
		<img src="<?php echo ting_search_cover_url($collection->objects[0], 'ting_search_carousel') ?>" />
		<span class="creator"><?php echo implode($collection->objects[0]->data->creator, ',')?></span>
		<span class="title"><?php echo implode($collection->objects[0]->data->title, ',')?></span>
	</a>
</li>