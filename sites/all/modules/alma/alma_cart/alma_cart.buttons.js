// $Id$

/**
 * Javascript helpers for Alma cart interaction buttons.
 */
Drupal.behaviors.almaCartButtons = function () {
  $("ul.alma-cart-buttons a").click(function (event) {
      // Make sure we grab the click.
      event.preventDefault();

      var $button = $(this);

      if (!$button.hasClass('disabled')) {
        // If the dialog div doesn't exist, create it.
        if ($('#alma-cart-dialog').length === 0) {
          $("#content-main").append('<div id="alma-cart-dialog"></div>');
        }

        // Make the request back to Drupal.
        $.post(this.href, {}, function (data) {
          var buttons, $count, message;
          // Message is overwritten by the data attribute.
          message = '';
          buttons = {};
          buttons[Drupal.t('Close')] = function () {
            $(this).dialog('close');
          };

          if (data) {
            message = data.message;
            if (data.status === 'success' && $button.hasClass('add-to-cart')) {
              buttons[Drupal.t('View cart…')] = function () {
                window.location = data.cart_link;
              };
              $count = $('#block-alma-user-account .cart .count');
              $count.text(parseInt($count.text(), 10) + 1);
            }
          }
          else {
            message = Drupal.t('An error occurred.');
          }

          $('#alma-cart-dialog')
            .text(message)
            .dialog({'title': data.title, 'buttons': buttons, 'autoOpen': false})
            .dialog("open");
        }, 'json');
      }

      return false;
  });
};

