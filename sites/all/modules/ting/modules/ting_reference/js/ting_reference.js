Drupal.behaviors.addTingReferenceAutocomplete = function(context)
{
	type = jQuery('.ting-reference-type-radio:checked').val();
	path = '/'+Drupal.settings.tingReference.autocomplete[type];
	jQuery('input.ting-reference-autocomplete').each(function(i, e)
	{
		var autocompleter = jQuery(e);
		
		autocompleter.autocomplete(path, {});
		autocompleter.result(function(event, data, formatted)
		{
			$(event.target).parent().siblings('.ting-object-id').val(data[1]).change();
		});

		autocompleter.parents('td:first').find('input.ting-reference-type-radio').focus(function()
		{
			type = jQuery(this).val();
			path = '/'+Drupal.settings.tingReference.autocomplete[type];
			autocompleter.setOptions({ url: path });
			autocompleter.flushCache();
		});

	});

}

Drupal.behaviors.showTingPreview = function(context)
{
	jQuery('input.ting-object-id').change(function()
	{
		var input = jQuery(this);
		jQuery.getJSON(	Drupal.settings.tingReference.previewUrl, 
										{ id: jQuery(this).val() },
										function(data) {
											collections = jQuery(Drupal.settings.tingReference.referenceTemplate).mapDirective({
												'.title': function(arg) { return (arg.context.objects[0].data.title) ? arg.context.objects[0].data.title.join(', ') : ''; },
												'.title[href]': 'collection.url',
												'.creator em': function(arg) { return (arg.context.objects[0].data.creator) ? arg.context.objects[0].data.creator.join(', ') : ''; },
												'.publication_date em': function(arg) { return (arg.context.objects[0].data.date) ? arg.context.objects[0].data.date.join(', ') : ''; },
												'.description p': function(arg) { return (arg.context.objects[0].data.description) ? arg.context.objects[0].data.description.join('</p><p>') : ''; },
												'.picture': function(arg) { return (arg.context.objects[0].additionalInformation) ? '<img src="'+arg.context.objects[0].additionalInformation.thumbnailUrl+'"/>' : ''; }
											});
											
											types = jQuery('.types', collections).mapDirective({
												'li': [ 'type <- collection.types',
																'type']
											});
									
											subjects = jQuery('.subjects', collections).mapDirective({
												'li': [	'subject <- collection.objects.0.data.subject', //Only render subjects from first object in collection
																'subject']
											});
									
											$('.types', collections).replaceWith(jQuery(types));
											$('.subjects', collections).replaceWith(jQuery(subjects));
											$p.compile(collections, 'ting-preview');
									
											input.parents('td:first').find('.ting-reference-preview').html($p.render('ting-preview', data));											
										});
	});
}