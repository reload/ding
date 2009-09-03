Drupal.behaviors.addTingAutocomplete = function(context)
{
	path = Drupal.settings.tingSearchAutocomplete.path;
	jQuery('input.ting-autocomplete').each(function(i, e)
	{
		var autocompleter = jQuery(e);
		
		autocompleter.autocomplete(path, 
															{ scrollHeight: 200, /* Consider moving dimensions to CSS for easier customization */ 
																width: 493,
																delay: 200,
																selectFirst: false,
																formatResult: function(data) { return data[1]; }
															}). result(function(event)
															{
																jQuery(event.target).addClass('ac_loading')
																										.parents('form:first').submit();
															});
	});	
}