// $Id$

/**
 * Search result handling function.
 */
Drupal.tingResult = function (searchResultElement, facetBrowserElement, result) {
	this.searchResultElement = searchResultElement;
	this.facetBrowserElement = facetBrowserElement;
	
	this.renderTingSearchResults = function (element, result) {
		$(element).find('ul,ol').html(result.result_html);
		Drupal.tingSearch.updateSummary($('#ting-search-summary'), result);
	};
	
	this.renderTingSearchResultPager = function (element, result) {
		//currentPage = ($.url.attr('page')) ? $.url.attr('page') : 1;
    var anchorVars, currentPage, totalPages, startPage, endPage, pages, p, paging, pager, pageNumberClasses;
		anchorVars = Drupal.getAnchorVars();
		currentPage = (anchorVars.page !== null) ? parseInt(anchorVars.page) : 1;
		totalPages = Math.ceil(result.count / result.resultsPerPage);

		if (totalPages > 1) {
			startPage = Math.max(1, (currentPage - Math.ceil((Drupal.settings.tingResult.pagesInPager - 1) / 2)));
			endPage = Math.min(totalPages, (currentPage + Math.ceil((Drupal.settings.tingResult.pagesInPager - 1) / 2)));

			pages = [];
			for (p = startPage; p <= endPage; p++) {
				pages.push(p);
			}
			
			paging = {	first: 1,
									current: currentPage,
									last: totalPages,
									pages: pages }
						
			pager = $(Drupal.settings.tingResult.pagerTemplate).mapDirective({
				'.prev[class]+' : function(arg) { return (arg.context.current == arg.context.first) ? 'hidden' : ''; },
				'.first' : 'first',
				'.first[class]+' : function(arg) { return ($.inArray(arg.context.first, arg.context.pages) > -1) ? 'hidden' : ''; },
				'.last' : 'last',
				'.last[class]+' : function(arg) { return ($.inArray(arg.context.last, arg.context.pages) > -1) ? 'hidden' : ''; },
				'.next[class]+' : function(arg) { return (arg.context.current == arg.context.last) ? 'hidden' : ''; },
				'li': 'page <- pages',
				'li[class]': function(arg) { return (arg.item == arg.context.current) ? 'current' : ''; },
				'li a': 'page'
			});
			
			$p.compile(pager, 'search-result-pager');
			pager = $p.render('search-result-pager', paging);
			
			pageNumberClasses = {
        '.first': 1,
				'.prev': currentPage - 1,
				'.next': currentPage + 1,
				'.last': totalPages
      };
			
			pager = $(pager);
			$.each(pageNumberClasses, function(i, e) {
				var page = pageNumberClasses[i];
				pager.find(i).click(function() {
					Drupal.updatePageUrl(page);
					Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
					return false;
				});
			});
			$(pager).find('ul a').click(function() {
				Drupal.updatePageUrl($(this).text());
				Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
				return false;
			});
			
			element = $(element);
			(element.next().size() > 0) ? element.next().replaceWith(pager) : element.after(pager);
		}
	};
	
	this.doUrlSearch = function() {
			//Start loading
			Drupal.tingSearch.tabLoading('ting');
			
			var vars = Drupal.getAnchorVars();
			vars.query = Drupal.settings.tingSearch.keys;
			
			$.getJSON(Drupal.settings.tingSearch.ting_url, vars, function(data) {
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
	};
	
	this.updatePageUrl = function(pageNumber) {
		var anchorVars = Drupal.getAnchorVars();
		anchorVars.page = pageNumber;
		Drupal.setAnchorVars(anchorVars);
	};

	this.updateSortUrl = function(sort) {
		var anchorVars = Drupal.getAnchorVars();
		anchorVars.sort = sort;
		delete anchorVars.page;
		Drupal.setAnchorVars(anchorVars);
	};

	this.renderTingSearchResults(searchResultElement, result);
	this.renderTingSearchResultPager(searchResultElement, result);

	$('#edit-ting-search-sort').val(Drupal.getAnchorVars().sort);
	$('#edit-ting-search-sort').change(function() {
		Drupal.updateSortUrl($(this).val());
		Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
	});
};

