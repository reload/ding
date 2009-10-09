// $Id$

Drupal.behaviors.tingSearchCarouselAdmin = function(context)
{
	Drupal.tingSearchCarouselAdmin.remove();
	Drupal.tingSearchCarouselAdmin.sort();
};

Drupal.tingSearchCarouselAdmin = {};

Drupal.tingSearchCarouselAdmin.remove = function () {
  $('.ting-search-carousel-search-wrap .remove').click(function (e) {
	$(this).parents('.form-item').slideUp('normal', function () 
	{ 
	  searches = $(this).parents('#ting-search-carousels');
	  $(this).remove();
	  Drupal.tingSearchCarouselAdmin.updateIndexes(searches);
  	});
	return false;
  });
};

Drupal.tingSearchCarouselAdmin.sort = function () {
  $('#ting-search-carousels').sortable({
	items: '.form-item',
	handle: '.sort',
	axis: 'y',
	update: function(event, ui) { Drupal.tingSearchCarouselAdmin.updateIndexes(ui.item.parents('#ting-search-carousels')); }
  });
};

Drupal.tingSearchCarouselAdmin.updateIndexes = function(searches)
{
	var i = 0;
	$(searches).find('.fieldset-content > .form-item').each(function()
	{
		
		formElements = $(this).find('input[name^=ting_search_carousel_searches]');
		if (formElements.size() > 0)
		{
			formElements.each(function()
			{
				name = $(this).attr('name').replace(/\[\d+\]/, '['+i+']');
				$(this).attr('name', name);
			});
			i++;
		}
	 });
}