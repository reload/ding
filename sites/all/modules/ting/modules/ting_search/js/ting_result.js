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
		jQuery(element).find('ul,ol').html(result.result_html);
		Drupal.tingSearch.updateSummary($('#ting-search-summary'), result);
	}
	
	this.renderTingSearchResultPager = function(element, result)
	{
		//currentPage = (jQuery.url.attr('page')) ? jQuery.url.attr('page') : 1;
		anchorVars = Drupal.getAnchorVars();
		currentPage = (anchorVars.page != null) ? new Number(anchorVars.page) : 1;
		totalPages = Math.ceil(result.count / result.resultsPerPage);

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
			//Start loading
			Drupal.tingSearch.tabLoading('ting');
			
			//Encode query and anchor values to make IE handle them correctly in the AJAX request
			query = encodeURIComponent(Drupal.settings.tingSearch.keys);

			vars = Drupal.getAnchorVars();
			anchorArray = new Array();
			for (v in vars)
			{
				anchorArray.push(v+'='+encodeURIComponent(vars[v]));
			}
			anchorString = anchorArray.join('&');
			
			var path = Drupal.settings.tingSearch.ting_url+'?query='+query+((anchorString) ? '&'+anchorString : '');
			jQuery.getJSON(path, function(data)
			{
				//Update tabs now that we have the result
	      Drupal.tingSearch.summary.ting = { count: data.count, page: data.page };
				Drupal.tingSearch.updateTabs('ting');
	      
				//Update search result and facet browser
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

