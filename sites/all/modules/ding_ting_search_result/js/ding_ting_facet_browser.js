Drupal.dingTingFacetBrowser = function(element, result)
{

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
			'h4': 'facet.name'
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
		jQuery(element).html($p.render('facet-groups', result));
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
		 		facetElement.attr('facet', t); 
		 		facetElement.find('.name').text(t);
		 		facetElement.find('.count').text(result.facets[f].terms[t]);
				
				facetElement.removeClass('hidden');
				if ((facetElement.size() > 0) && 
		 				jQuery.inArray(t, selectedTerms) > -1)
		 		{
		 			facetElement.addClass('selected')
		 		}
		 		else
		 		{
		 			facetElement.removeClass('selected');
		 		}
		 	});

			jQuery('li:gt('+Object.keys(result.facets[f].terms).length+')', facetElements).removeClass('selected').addClass('hidden');;
		}
	}
	
	this.updateSearchResults = function(result)
	{
		records = jQuery('<ol><li>'+Drupal.settings.dingTingSearchResult.recordTemplate+'</li></ol>').mapDirective({
			'li': 'record <- records',
			'.title': function(arg) { return (arg.item.data.title) ? arg.item.data.title.join(', ') : ''; },
			'.creator em': function(arg) { return (arg.item.data.creator) ? arg.item.data.creator.join(', ') : ''; },
			'.publication_date em': function(arg) { return (arg.item.data.date) ? arg.item.data.date.join(', ') : ''; },
			'.description p': function(arg) { return (arg.item.data.description) ? arg.item.data.description.join('</p><p>') : ''; }
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

		//TODO remove CSS specification from JS 
		jQuery('.search-results ol').replaceWith($p.render('search-result', result));
	}
	
	this.initCarousel = function(element)
	{
		jQuery(element).children().addClass('jcarousel-skin-ding-facet-browser').jcarousel();	
	}
	
	this.renderResizeButton = function(element)
	{
		button = jQuery('<a class="resize" href="">'+Drupal.settings.dingTingSearchResult.resizeButtonText+'</a>');
		element = jQuery(element);
		(element.height() < Drupal.getFacetHeight(element)) ? button.addClass('expand') : button.addClass('disabled');
		
		jQuery(element).append(button);
	}
	
	this.bindResizeEvent = function(element)
	{
		var element = jQuery(element);
		var baseHeight = element.height();
		jQuery('.expand', jQuery(element)).toggle(function()
		{						
			jQuery('.jcarousel-clip-horizontal', element).animate({ 'height': (Drupal.getFacetHeight()+20)+'px' }, 1000, 'swing', function()
			{
				jQuery('.resize', element).toggleClass('expand').toggleClass('contract');
			});
			jQuery('.jcarousel-prev, .jcarousel-next', element).animate({ 'top': ((Drupal.getFacetHeight()+20)/2)+'px' }, 1000, 'swing');
			
		}, function()
		{
			jQuery('.jcarousel-clip-horizontal', element).animate({ 'height': baseHeight+'px' }, 1000, 'swing', function()
			{
				jQuery('.resize', element).toggleClass('expand').toggleClass('contract');
			});
			jQuery('.jcarousel-prev, .jcarousel-next', element).animate({ 'top': (baseHeight/2)+'px' }, 1000, 'swing');
		});
	}
	
	this.bindSelectEvent = function(element)
	{
		jQuery('.facets li', element).unbind('click');
		jQuery('.facets li:not(.hidden)', element).click(function()
		{
			clicked = $(this);
			clicked.toggleClass('selected');
			Drupal.updateSelectedUrl(element);			
			Drupal.doSelectedSearch(element);
		});
	}
	
	this.doSelectedSearch = function(element)
	{
			facetString = 'facets=';
			jQuery('li.selected', element).each(function()
			{
				facetString += $(this).attr('facet-group')+':'+$(this).attr('facet')+';';
			});
			
			path = jQuery.url.attr('path')+'?'+jQuery.url.attr('query')+'&'+facetString; 
			jQuery.getJSON(path, function(data)
			{
				Drupal.updateSearchResults(data);
				Drupal.updateFacetBrowser(element, data);
				Drupal.bindSelectEvent(element);
			});
	}
	
	this.updateSelectedUrl = function(element)
	{
		anchor = 'facets=';
		jQuery('.selected', element).each(function(i, e)
		{
			anchor += jQuery(e).attr('facet-group')+':'+jQuery(e).attr('facet')+';';
		});
		
		url = window.location.href;
		window.location.href = url.substring(0, url.lastIndexOf('#'))+'#'+anchor;
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
	this.renderFacetBrowser(element, result);
	if (this.updateSelectedFacetsFromUrl(element))
	{
		this.doSelectedSearch();
	}

	this.initCarousel(element);
	this.renderResizeButton(element);
	this.bindResizeEvent(element);
	this.bindSelectEvent(element);
}
