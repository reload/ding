// $Id$

/**
 * Javascript helpers for Alma cart interaction buttons.
 */

Drupal.behaviors.almaCartButtons = function () {
  $("ul.alma-cart-buttons a").click(function (event) {
      // Make sure we grab the click.
      event.preventDefault();

      // If the dialog div doesn't exist, create it.
      if ($('#alma-cart-dialog').length == 0) {
        $("#content-main").append('<div id="alma-cart-dialog"></div>');
      }

      // Make the request back to Drupal.
      $.post(this.href, {}, function (data) {
        var buttons = {};
        if (data.status != 'success') {
          buttons[Drupal.t('Close')] = function () { $(this).dialog('close'); };
        }
        else {
          buttons[Drupal.t('Close')] = function () { $(this).dialog('close'); };
          buttons[Drupal.t('View cart…')] = function () { window.location = data.cart_link };
        }

        $('#alma-cart-dialog')
          .text(data.message)
          .dialog({'title': data.title, 'buttons': buttons, 'autoOpen': false})
          .dialog("open");
      }, 'json');

      return false;
  });
}

