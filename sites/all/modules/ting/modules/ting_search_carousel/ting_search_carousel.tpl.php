<?php krumo($searches); ?>
<div class="ting-search-carousel">
	<ul class="search-results">
	<?php for ($i = 0; $i < 4; $i++) :?>
		<li class="<?php echo ($i == 0) ? 'active' : ''; ?>">
		<ul>
		<?php for ($j = 0; $j < 6; $j++) :?>
			<li><img src="" /> <span class="creator">Creator</span> <span
				class="title">Title</span></li>
				<?php endfor; ?>
		</ul>
		</li>
		<?php endfor; ?>
	</ul>
	
	<ul class="search-controller">
		<?php foreach ($searches as $i => $search) : ?>
			<li class="<?php echo ($i == 0) ? 'active' : ''; ?>">
				<a href="#"><?php echo $search['title'] ?></a>
			</li>
		<?php endforeach; ?>
	</ul>
</div>
