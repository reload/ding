<div class="ting_reference">
  <div class="picture">
  </div>
  <div class="info">
		<h3 class="title">
			<a class="title" href="<?php echo url('ting/collection') ?>"><?php echo implode(', ', $collection->objects[0]->data->title) ?></a>
		</h3>
		<div class="meta">
			<?php if ($collection->objects[0]->data->creator) : ?>
				<span class="creator">
					<?php echo t('By %creator_name%', array('%creator_name%' => implode(', ', $collection->objects[0]->data->creator))) ?>
				</span>
			<?php endif; ?>
			<?php if ($collection->objects[0]->data->date) : ?>
				<span class="publication_date">
					<?php echo t('(%publication_date%)', array('%publication_date%' => implode(', ', $collection->objects[0]->data->date))) /* TODO: Improve date handling, localizations etc. */ ?>
				</span>
			<?php endif; ?>
		</div>
		<div class="types">
			<h4><? echo t('Material types:') ?></h4>
			<?php foreach ($collection->objects[0]->data->type as $type) ?>
			<ul>
				<li class="available"><?php echo $type; ?></li>
				<!--<li class="out">CD</li>
				<li class="reserved">Lydbog</li>-->
			</ul>
		</div>
		<?php if ($collection->objects[0]->data->abstract) : ?>
		<div class="abstract">
			<p>
				<?php echo implode('</p><p>'.$collection->objects[0]->data->abstract) ?>
			</p>
		</div>
		<?php endif; ?>
		<?php if ($collection->objects[0]->data->subject) : ?>
			<div class="subjects">
				<h4><?php echo t('Subjects:') ?></h4>
				<ul>
					<?php foreach ($collection->objects[0]->data->subject as $subject) : ?>
						<li><?php echo $subject ?></li>
					<?php endforeach; ?>
				</ul>	
			</div>
		<?php endif; ?>
	</div>
</div>
