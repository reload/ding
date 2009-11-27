// $Id: path_redirect.vertical_tabs.node_form.js,v 1.1.2.1 2009/11/27 02:58:33 davereid Exp $

Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.path_redirect = function() {
  if ($('table.path-redirects tbody td.path-redirect-none').size()) {
    return Drupal.t('No redirects');
  }
  else {
    var redirects = $('table.path-redirects tbody tr').size();
    return Drupal.formatPlural(redirects, '1 redirect', '@count redirects');
  }
}
