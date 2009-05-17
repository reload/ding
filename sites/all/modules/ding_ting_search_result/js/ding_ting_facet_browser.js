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
			'.facet-group[class]': function(arg) {
				firstLast = (Object.keys(arg.items).first() == arg.item.name) ? 'first' : 
       							((Object.keys(arg.items).last() == arg.item.name) ? 'last' : '');
       	return firstLast;
			},
			'h4': 'facet.name',
			
		});
		
		facetTerms = jQuery('.facets', facetGroups).mapDirective({
			'li': 'term <- facet.terms',
			'li[class]': function(arg) {
				return 'facet-'+arg.pos + ' ' + (((Object.keys(arg.items).indexOf(arg.pos.toString()) % 2) == 0) ? ' odd' : ' even' ); 
			},
			'.name': function(arg) { return arg.pos; },
			'.count': function(arg) { return arg.item; }
			
		});
		$('.facets', facetGroups).html(jQuery('li', facetTerms));
		$p.compile(facetGroups, 'facet-groups');
		jQuery(element).html($p.render('facet-groups', result));
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
}
