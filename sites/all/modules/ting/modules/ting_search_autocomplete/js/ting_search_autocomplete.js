/**
 * JavaScript behavior for the autocomplete widget.
 */
Drupal.behaviors.addTingAutocomplete = function(context) {
  $('input.ting-autocomplete').each(function (index, domElement) {
    $(domElement)
      .autocomplete(Drupal.settings.tingSearchAutocomplete.path, {
        scrollHeight: 200,
        width: 493,
        delay: 200,
        selectFirst: false,
        matchCase: true,
        formatResult: function (data) { return data[1]; }
      })

      .result(function(event) {
        $(event.target)
          .addClass('ac_loading')
          .parents('form:first')
          .submit();
      });
  });
}

