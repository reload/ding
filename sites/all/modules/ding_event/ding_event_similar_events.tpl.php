<?php
// $Id$

/**
 * @file ding_event_similar_events.tpl.php
 * Template to display similar events, usually in the context of a Panel.
 */
?>
<ul class="similar-events">
<?php foreach ($events as $node): ?>
  <li><?php print l($node->title, 'node/' . $node->nid); ?></li>
<?php endforeach; ?>
</ul>

