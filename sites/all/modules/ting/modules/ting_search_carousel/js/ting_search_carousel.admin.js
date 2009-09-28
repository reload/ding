// $Id$

Drupal.behaviors.tingSearchCarousel = function () {
  $('.ting-search-carousel-search-wrap .remove').click(function (e) {
	  $(this).parents('.form-item').slideUp();
	  return false;
  });
};

