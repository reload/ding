Drupal.behaviors.addTingReferenceAutocomplete = function(context)
{
	type = $('.ting-reference-type-radio:checked').val();
	path = '/'+Drupal.settings.tingReference.autocomplete[type];
	jQuery('input.ting-reference-autocomplete').each(function(i, e)
	{
		var autocompleter = $(e);
		
		autocompleter.autocomplete(path, {});
		autocompleter.result(function(event, data, formatted)
		{
			$(event.target).parent().siblings('.ting-object-id').val(data[1]);
		});

		autocompleter.parents('td:first').find('input.ting-reference-type-radio').focus(function()
		{
			type = $(this).val();
			path = '/'+Drupal.settings.tingReference.autocomplete[type];
			autocompleter.setOptions({ url: path });
			autocompleter.flushCache();
		});

	});

}