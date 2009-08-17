<?php
// $Id$

/**
 * @file alma_user_status_block.tpl.php
 * Template for the user status block.
 */
?>
<div id="account-profile" class="clearfix">
	<div class="user">

		<div class="logout">
			<?php print l(t('log out'), 'logout', $options= array('attributes' => array('class' =>'logout')) );  ?>	
		</div>

		<h5><?php print t('Welcome'); ?></h5>
		<div class="username">
 			
			<?php print l($user->name, 'patron/profile', $options= array('attributes' => array('class' =>'username')) );  ?>												
		</div>

	</div>

	<div class="cart">
		<div><?php print $cart_count; ?></div>
		<?php print l('Go to cart', 'patron/cart'); ?>
	</div>

	<ul>
		<li>
			<div class="content">
				<?php print l(t("Borrowed materials") . ' <span class="count">' . $borrowed_count . '</span>', 'patron/status', array('html' => TRUE)); ?>
			</div>
			<div class="status"><span class="warning">!!</span></div>
		</li>
		<li>
			<div class="content">
				<?php print l(t("Reservations") . ' <span class="count">' . $reservation_count . '</span>', 'patron/status', array('html' => TRUE)); ?>
			</div>
			<div class="status"><span class="ok">ok</span></div>
		</li>
	</ul>
</div>

