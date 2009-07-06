Drupal.tingResult = function(searchResultElement, facetBrowserElement, result)
{
	this.searchResultElement = searchResultElement;
	this.facetBrowserElement = facetBrowserElement;
	
	this.getAnchorVars = function()
	{
		anchorValues = {};
		
		anchor = jQuery.url.attr('anchor');
		anchor = (anchor == null) ? '' : anchor;
		
		anchor = anchor.split('&');
		for(a in anchor)
		{
			keyValue = anchor[a].split('=');
			if (keyValue.length > 1)
			{
				anchorValues[keyValue[0]] = keyValue[1];
			}
		}
		
		return anchorValues;
	}
	
	this.setAnchorVars = function(vars)
	{
		anchorArray = new Array();
		for (v in vars)
		{
			anchorArray.push(v+'='+vars[v]);
		}
		anchorString = anchorArray.join('&');
		
		url = window.location.href;
		window.location.href = url.substring(0, url.lastIndexOf('#'))+'#'+anchorString;		
	}

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
		//currentPage = (jQuery.url.attr('page')) ? jQuery.url.attr('page') : 1;
		anchorVars = Drupal.getAnchorVars();
		currentPage = (anchorVars.page != null) ? new Number(anchorVars.page) : 1;
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
			console.log(paging);
						
			pager = jQuery(Drupal.settings.tingResult.pagerTemplate).mapDirective({
				'.prev[class]+' : function(arg) { return (arg.context.current == arg.context.first) ? 'hidden' : '' },
				'.first' : 'first',
				'.first[class]+' : function(arg) { return (jQuery.inArray(arg.context.first, arg.context.pages) > -1) ? 'hidden' : '' },
				'.last' : 'last',
				'.last[class]+' : function(arg) { return (jQuery.inArray(arg.context.last, arg.context.pages) > -1) ? 'hidden' : ''  },
				'.next[class]+' : function(arg) { return (arg.context.current == arg.context.last) ? 'hidden' : '' },
				'li': 'page <- pages',
				'li[class]': function(arg) { return (arg.item == arg.context.current) ? 'current' : ''  },
				'li a': 'page'
			});
			
			$p.compile(pager, 'search-result-pager');
			pager = $p.render('search-result-pager', paging);
			
			pageNumberClasses = {	'.first': 1,
														'.prev': currentPage - 1,
														'.next': currentPage + 1,
														'.last': totalPages };
			
			pager = jQuery(pager);
			jQuery.each(pageNumberClasses, function(i, e)
			{
				var page = pageNumberClasses[i];
				pager.find(i).click(function()
				{
					Drupal.updatePageUrl(page);
					Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
					return false;
				});
			});
			jQuery(pager).find('ul a').click(function()
			{
				Drupal.updatePageUrl(jQuery(this).text());
				Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
				return false;	
			});
			
			element = jQuery(element);
			(element.next().size() > 0) ? element.next().replaceWith(pager) : element.after(pager);
		}
	}
	
	this.doUrlSearch = function()
	{
			var path = Drupal.settings.tingSearch.ting_url+'?query=dc.title:'+Drupal.settings.tingSearch.keys+'&'+jQuery.url.attr('anchor'); 
			jQuery.getJSON(path, function(data)
			{
				Drupal.renderTingSearchResults(Drupal.searchResultElement, data);
				Drupal.renderTingSearchResultPager(Drupal.searchResultElement, data);
				Drupal.updateFacetBrowser(Drupal.facetBrowserElement, data);
				Drupal.bindSelectEvent(Drupal.facetBrowserElement, searchResultElement);
				Drupal.updateSelectedFacetsFromUrl(Drupal.facetBrowserElement);
			});
	}
	
	this.updatePageUrl = function(pageNumber)
	{
		anchorVars = Drupal.getAnchorVars();
		anchorVars.page = pageNumber;
		Drupal.setAnchorVars(anchorVars);
	}
	
	this.renderTingSearchResults(searchResultElement, result);
	this.renderTingSearchResultPager(searchResultElement, result);
	
}