<ul>
	<li>

    <div class="picture">
      image...
    </div>
    <div class="record">

  		<div class="types">
  			<?php foreach ($record->data->type as $type) ?>
  			<ul>
  				<li class="available"><?php echo $type; ?></li>
  				<li class="out">CD</li>
  				<li class="reserved">Lydbog</li>
  			</ul>
  		</div>

  		<h3 class="title"><?php echo implode(', ', $record->data->title) ?></h3>

  		<div class="meta">
  			<?php if ($record->data->creator) : ?>
  				<span class="creator">
  					<?php echo t('By %creator_name%', array('%creator_name%' => implode(', ', $record->data->creator))) ?>
  				</span>
  			<?php endif; ?>
  			<?php if ($record->data->date) : ?>
  				<span class="publication_date">
  					<?php echo t('(%publication_date%)', array('%publication_date%' => implode(', ', $record->data->date))) /* TODO: Improve date handling, localizations etc. */ ?>
  				</span>
  			<?php endif; ?>
  		</div>
		

  		<?php if ($record->data->description) : ?>
  		<div class="description">
  			<p>
  				<?php echo implode('</p><p>'.$record->data->description) ?>
  			</p>
  		</div>
  		<?php endif; ?>

  		<?php if ($record->data->subject) : ?>
  			<div class="subjects">
  				<h4><?php echo t('Subjects:') ?></h4>
  				<ul>
  					<?php foreach ($record->data->subject as $subject) : ?>
  						<li><?php echo $subject ?></li>
  					<?php endforeach; ?>
  				</ul>	
  			</div>
  		<?php endif; ?>

    </div>

	</li>
</ul>
