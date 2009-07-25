<?php
// $Id$

/**
 * @file alma_user_status_block.tpl.php
 * Template for the user status block.
 */
?>
<div class="logout-link"><?php print l('Log out', 'logout'); ?></div>

<div class="welcome-username">
  <span class="welcome"><?php print t('Welcome'); ?></span>
  <span class="user-name"><?php print l($user->name, 'patron/profile'); ?></span>
</div>

<div class="alma-patron-status">
  <div class="cart-status">
    <span class="count"><?php print $cart_count; ?></span>
    <?php print l('Go to cart', 'patron/cart'); ?>
  </div>
  <ul class="materials">
    <li class="borrowed">
      <?php print l(t("Borrowed materials") . ' <span class="count">' . $borrowed_count . '</span>', 'patron/status', array('html' => TRUE)); ?>
    </li>
    <li class="reservations">
      <?php print l(t("Reservations") . ' <span class="count">' . $reservation_count . '</span>', 'patron/status', array('html' => TRUE)); ?>
    </li>
  </ul>
</div>

