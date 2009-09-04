Drupal.tingFacetBrowser = function(facetBrowserElement, searchResultElement, result)
{
	this.searchResultElement = searchResultElement;
	this.facetBrowserElement = facetBrowserElement;

	this.renderFacetBrowser = function(element, result)
	{
		//The base markup for the browser
		template = jQuery('<ol class="facet-groups">'+
												'<li class="facet-group">'+
													'<h4></h4>'+
													'<ol class="facets">'+
														'<li><span class="name"></span>(<span class="count"></span>)</li>'+
													'</ol>'+
												'</li>'+
											'</ol>');
		
		facetGroups = jQuery(template).mapDirective({
			'.facet-group': 'facet <- facets',
			'.facet-group[facet-group]': 'facet.name',
			'.facet-group[class]+': function(arg) {
				facets = Object.keys(arg.items);
				firstLast = (facets[0] == arg.item.name) ? ' first' : 
       							((facets[facets-length-1] == arg.item.name) ? ' last' : '');
       	return firstLast;
			},
			'h4': function(arg) { return (Drupal.settings.tingResult.facetNames[arg.item.name]) ? Drupal.settings.tingResult.facetNames[arg.item.name] : arg.item.name; }
		});
		
		facetTerms = jQuery('.facets', facetGroups).mapDirective({
			'li': 'term <- facet.terms',
			'li[class]+': function(arg) {
				return (((jQuery.inArray(arg.pos.toString(), Object.keys(arg.items)) % 2) == 0) ? ' odd' : ' even' ); 
			},
			'li[facet]': function(arg) { return arg.pos },
			'li[facet-group]': 'facet.name',
			'.name': function(arg) { return arg.pos; },
			'.count': function(arg) { return arg.item; }
		});
		
		$('.facets', facetGroups).html(jQuery('li', facetTerms));
		$p.compile(facetGroups, 'facet-groups');
		
		facetBrowser = jQuery($p.render('facet-groups', result));

		//Add additional empty list items to make evenly sized lists
		facets = jQuery('.facets', facetBrowser);
		numFacets = facets.map(function(i, e)
		{
			return jQuery('li', e).size();
		});
		var maxFacets = Math.max.apply(Math, jQuery.makeArray(numFacets));

		facets.each(function(i, e)
		{
			facetElement = jQuery('li:first', e);
			for(i = jQuery('li', e).size(); i < maxFacets; i++)
			{
				jQuery(e).append(facetElement.clone().removeClass().addClass((((i % 2) == 0) ? 'odd' : 'even')).addClass('hidden'));
			}
		});
		
		jQuery(element).html(facetBrowser);
		
		this.resizeFacets(element);
	}
	
	this.updateFacetBrowser = function(element, result)
	{
		for (f in result.facets)
		{
			var facetElements = jQuery('.facet-group[facet-group='+f+']', element);
		 	var selectedTerms = jQuery('.selected', facetElements).map(function(i, e)
		 	{
		 		return jQuery(e).attr('facet');
		 	});
		 	
		 	jQuery.each(Object.keys(result.facets[f].terms), function (i, t)
		 	{
		 		facetElement = jQuery('li:eq('+i+')', facetElements);
		 		//update facet values
		 		facetElement.attr('facet', t); 
		 		facetElement.find('.name').text(t);
		 		facetElement.find('.count').text(result.facets[f].terms[t]);
				
				//display facet and determine whether it is selected
				facetElement.removeClass('hidden');
				((facetElement.size() > 0) && jQuery.inArray(t, selectedTerms) > -1) ? 	facetElement.addClass('selected') : facetElement.removeClass('selected');
		 	});

			//hide and unselect all superflous facets
			jQuery('li:gt('+Object.keys(result.facets[f].terms).length+')', facetElements).removeClass('selected').addClass('hidden');
		}
		
		this.resizeFacets(element);
	}
	
	this.resizeFacets = function(element)
	{
		setTimeout(function() { 
			jQuery('.facets:first li', element).each(function(i, e)
			{
				facets = jQuery('.facets li:nth-child('+(i+1)+')');
				heights = facets.map(function(i, e) { jQuery(e).attr('height', jQuery(e).innerHeight()); return jQuery(e).height(); });
				var maxHeight = Math.max.apply(Math, jQuery.makeArray(heights));
				facets.each(function(i, e)
				{
					if (jQuery(e).height() < maxHeight)
					{
						jQuery(e).css('line-height', maxHeight+'px');
					}
				})
			});
		}, 0);
	}
	
	this.initCarousel = function(element)
	{
		jQuery(element).children().addClass('jcarousel-skin-ding-facet-browser').jcarousel();	
	}
	
	this.renderResizeButton = function(element)
	{
		button = jQuery('<a class="resize" href="">'+Drupal.settings.tingResult.resizeButtonText+'</a>');
		element = jQuery(element);
		(element.height() < Drupal.getFacetHeight(element)) ? button.addClass('expand') : button.addClass('disabled');
		
		jQuery(element).append(button);
	}
	
	this.bindResizeEvent = function(element)
	{
		var element = jQuery(element);
		var baseHeight = element.height();
		var headerHeight = jQuery('H4', element).height();
		var resizeButton = jQuery('.resize', element);
		
		jQuery('.expand', jQuery(element)).toggle(function()
		{						
			resizeButton.css('top', resizeButton.position().top+'px');
			jQuery('.jcarousel-clip-horizontal', element).animate({ 'height': (Drupal.getFacetHeight()+headerHeight)+'px' }, 1000, 'swing', function()
			{
				jQuery('.resize', element).toggleClass('expand').toggleClass('contract');
			});
			jQuery('.resize', element).animate({ 'top': (Drupal.getFacetHeight()+headerHeight-13)+'px' }, 1000, 'swing');
			jQuery('.jcarousel-prev, .jcarousel-next', element).animate({ 'top': ((Drupal.getFacetHeight()+headerHeight)/2)+'px' }, 1000, 'swing');
			
		}, function()
		{
			jQuery('.jcarousel-clip-horizontal', element).animate({ 'height': baseHeight+'px' }, 1000, 'swing', function()
			{
				jQuery('.resize', element).toggleClass('expand').toggleClass('contract');
			});
			jQuery('.resize', element).animate({ 'top': (baseHeight-10)+'px' }, 1000, 'swing' );
			jQuery('.jcarousel-prev, .jcarousel-next', element).animate({ 'top': (baseHeight/2)+'px' }, 1000, 'swing');
		});
	}
	
	this.bindSelectEvent = function(facetBrowserElement, searchResultElement)
	{
		jQuery('.facets li', facetBrowserElement).unbind('click');
		jQuery('.facets li:not(.hidden)', facetBrowserElement).click(function()
		{
			clicked = $(this);
			clicked.toggleClass('selected');
			Drupal.updateSelectedUrl(facetBrowserElement);			
			Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
		});
	}
	
	this.updateSelectedUrl = function(element)
	{
		facets = '';
		jQuery('.selected', element).each(function(i, e)
		{
			facets += jQuery(e).attr('facet-group')+':'+jQuery(e).attr('facet')+';';
		});
		if (facets.length > 0)
		{
			Drupal.setAnchorVars({ 'facets': facets });
		}
		
	}
	
	this.updateSelectedFacetsFromUrl = function(element)
	{
		if (jQuery.url.attr('anchor'))
		{
			match = jQuery.url.attr('anchor').match('facets=(([^:]*:[^;]*;)+)');
			if (match && match.length > 1)
			{
				facets = match[1].split(';');
				for (f in facets)
				{
					f = facets[f].split(':');
					if (f.length > 1)
					{
						jQuery('[facet-group='+f[0]+'][facet='+f[1]+']', element).addClass('selected');
					}
				}
			}
		}
		return jQuery('.selected', element).size() > 0;
	}
	
	this.getFacetHeight = function(element)
	{
		var maxHeight = 0;
		jQuery('.facets', element).each(function()
		{
			maxHeight = Math.max(maxHeight, jQuery(this).height());
		});
		return maxHeight;
	}
	
	//initialization
	this.renderFacetBrowser(facetBrowserElement, result);
	if (this.updateSelectedFacetsFromUrl(facetBrowserElement))
	{
		this.doUrlSearch(facetBrowserElement, searchResultElement);
	}

	this.initCarousel(facetBrowserElement);
	this.renderResizeButton(facetBrowserElement);
	this.bindResizeEvent(facetBrowserElement);
	this.bindSelectEvent(facetBrowserElement, searchResultElement);
}
