// $Id$

Drupal.behaviors.dingAdminNodeForm = function () {
  // When node title changes, set the menu title accordingly if it has
  // not been filled out yet.
  $("#edit-title-wrapper input.form-text").change(function () {
    if ($("#edit-menu-link-title").val() == "") {
      $("#edit-menu-link-title").val($(this).val());
    }
  });
};

