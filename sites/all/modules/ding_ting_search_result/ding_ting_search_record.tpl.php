<h3><?php echo implode(', ', $record->data->title) ?></h3>
<div class="meta">
	<?php if ($creator = $record->data->creator) : ?>
	<span class="creator">
		<?php echo t('By %creator_name%', array('%creator_name%' => implode(', ', $creator))) ?>
	</span>
	<?php endif; ?>
	<span class="publication_date">
		<?php echo t('(%publication_date%)', array('%publication_date%' => implode(', ', $record->data->date))) /* TODO: Improve date handling, localizations etc. */ ?>
	</span>
</div>
<div class="types">
	<h4><? echo t('Material types:') ?></h4>
	<ul>
		<li class="available">Bog</li>
		<li class="out">CD</li>
		<li class="reserved">Lydbog</li>
	</ul>
</div>
<div class="description">
	<p><?php echo implode('</p><p>'.$record->data->description) ?></p>
</div>
<?php if ($subjects = $record->data->subject) : ?>
	<div class="topics">
		<h4><?php echo t('Subjects:') ?></h4>
		<ul>
			<?php foreach ($subjects as $subject) : ?>
				<li><?php echo $subject ?></li>
			<?php endforeach; ?>
		</ul>	
	</div>
<?php endif; ?>
