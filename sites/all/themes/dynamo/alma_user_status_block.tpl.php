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
			<?php print l(t('log out'), 'logout', array('attributes' => array('class' =>'logout'))); ?>
		</div>

		<h5><?php print t('Welcome'); ?></h5>
		<div class="username">
			<?php print l($display_name, $profile_link, array('attributes' => array('class' =>'username')));  ?>
		</div>

	</div>
	<?php if ($user_status_available): ?>
		<div class="cart">
	    <div class="count"><?php print $cart_count; ?></div>
	    <?php print l('Go to cart', 'user/' . $user->uid . '/cart'); ?>
		</div>

		<ul>
	    <li>
	      <div class="content">
					<?php print l(t("Loans") . ' <span class="count">' . $user_status['loan_count'] . '</span>', 'user/'. $user->uid . '/status', array('html' => TRUE)); ?>
				</div>
				<div class="status"><span class="warning">!!</span></div>
	    </li>
	    <li>
				<div class="content">
	      <?php print l(t("Reservations") . ' <span class="count">' . $user_status['reservation_count'] . '</span>', 'user/'. $user->uid . '/status', array('html' => TRUE)); ?>
				</div>
				<div class="status"><span class="ok">ok</span></div>
	    </li>
	</ul>
	<?php else: ?>
	  <div class="status-unavailable">
	    <?php print $status_unavailable_message; ?>
	  </div>
	<?php endif; ?>
</div>

