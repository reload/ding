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
	
	this.renderTingSearchResultPager = function(element, result)
	{
		currentPage = (jQuery.url.attr('page')) ? jQuery.url.attr('page') : 1;
		totalPages = Math.ceil(result.numTotalRecords / Drupal.settings.tingResult.resultsPerPage);
		
		if (totalPages > 1)
		{
			startPage = Math.max(1, (currentPage - Math.ceil((Drupal.settings.tingResult.pagesInPager - 1) / 2)))
			endPage = Math.min(totalPages, (currentPage + Math.ceil((Drupal.settings.tingResult.pagesInPager - 1) / 2)))

			pages = new Array();
			for (p = startPage; p <= endPage; p++)
			{
				pages.push(p);
			}
			paging = {	first: 1,
									current: currentPage,
									last: totalPages,
									pages: pages }
						
			pager = jQuery(Drupal.settings.tingResult.pagerTemplate).mapDirective({
				'.prev[class]+' : function(arg) { return (arg.context.first == arg.context.pages[0]) ? 'hidden' : '' },
				'.first' : 'first',
				'.first[class]+' : function(arg) { return (jQuery.inArray(arg.context.first, arg.context.pages) > -1) ? 'hidden' : '' },
				'.last' : 'last',
				'.last[class]+' : function(arg) { return (jQuery.inArray(arg.context.last, arg.context.pages) > -1) ? 'hidden' : ''  },
				'.next[class]+' : function(arg) { return (arg.context.last == arg.context.pages[arg.context.pages.length - 1]) ? 'hidden' : '' },
				'li': 'page <- pages',
				'li[class]': function(arg) { return (arg.item == arg.context.current) ? 'current' : ''  },
				'li a': 'page'
			});
			
			$p.compile(pager, 'search-result-pager');
			jQuery(element).append($p.render('search-result-pager', paging));
		}
	}
	
	this.renderTingSearchResults(element, result);
	this.renderTingSearchResultPager(element, result);
	
}