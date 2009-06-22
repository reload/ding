Drupal.tingResult = function(element, result)
{

	this.renderTingSearchResults = function(element, result)
	{
		records = jQuery(Drupal.settings.tingResult.recordTemplate).mapDirective({
			'li': 'record <- records',
			'.title': function(arg) { return (arg.item.data.title) ? arg.item.data.title.join(', ') : ''; },
			'.creator em': function(arg) { return (arg.item.data.creator) ? arg.item.data.creator.join(', ') : ''; },
			'.publication_date em': function(arg) { return (arg.item.data.date) ? arg.item.data.date.join(', ') : ''; },
			'.description p': function(arg) { return (arg.item.data.description) ? arg.item.data.description.join('</p><p>') : ''; },
			'.picture': function(arg) { return (arg.item.additionalInformation) ? '<img src="'+arg.item.additionalInformation.thumbnailUrl+'"/>' : ''; }
		});
		
		types = jQuery('.types', records).mapDirective({
			'li': ['type <- record.data.type',
						 'type' ]
		});

		subjects = jQuery('.subjects', records).mapDirective({
			'li': [	'subject <- record.data.subject',
							'subject']
		});
		
		$('.types', records).replaceWith(jQuery(types));
		$('.subjects', records).replaceWith(jQuery(subjects));
		$p.compile(records, 'search-result');

		jQuery(element).html($p.render('search-result', result));
	}
	
	this.renderTingSearchResults(element, result);
	
}