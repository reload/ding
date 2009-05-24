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
				firstLast = (Object.keys(arg.items).first() == arg.item.name) ? ' first' : 
       							((Object.keys(arg.items).last() == arg.item.name) ? ' last' : '');
       	return firstLast;
			},
			'h4': 'facet.name',
			
		});
		
		facetTerms = jQuery('.facets', facetGroups).mapDirective({
			'li': 'term <- facet.terms',
			'li[class]+': function(arg) {
				return (((Object.keys(arg.items).indexOf(arg.pos.toString()) % 2) == 0) ? ' odd' : ' even' ); 
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
		 	
		 	Object.keys(result.facets[f].terms).each(function (t, i)
		 	{
		 		facetElement = jQuery('li:eq('+i+')', facetElements);
		 		facetElement.attr('facet', t); 
		 		facetElement.find('.name').text(t);
		 		facetElement.find('.count').text(result.facets[f].terms[t]);
					
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

			//TODO: Remove obsolete facets if result has fewer than the current list
			//or add extra if result has more
		}
	}
	
	this.initCarousel = function(element)
	{
		jQuery(element).children().addClass('jcarousel-skin-ding-facet-browser').jcarousel();	
	}
	
	this.renderResizeButton = function(element)
	{
		button = jQuery('<a class="resize" href="">Resize</a>');
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
		jQuery('.facets li', element).click(function()
		{
			clicked = $(this);
			clicked.toggleClass('selected');
			Drupal.doSelectedSearch(element);
			Drupal.updateSelectedUrl(element);			
		});
	}
	
	this.doSelectedSearch = function(element)
	{
			facetString = 'facets=';
			jQuery('li.selected', element).each(function()
			{
				facetString += $(this).attr('facet-group')+':'+$(this).attr('facet')+';';
			});
			
			path = jQuery.url.attr('path')+'?'+facetString; 
			jQuery.getJSON(path, function(data)
			{
				Drupal.updateFacetBrowser(element, data);
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
		window.location.href = url.substring(0, url.lastIndexOf('#'))+'#'+anchor+'&';
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
	
	this.renderFacetBrowser(element, result);
	this.initCarousel(element);
	this.renderResizeButton(element);
	this.bindResizeEvent(element);
	this.bindSelectEvent(element);
}
