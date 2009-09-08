$(document).ready(function() {

		//password iphone thingie
//		$('input:password').dPassword({
		$('#edit-name').dPassword({
				duration: 2000,
				prefix: 'my_'
		});

	//label
	  $("label").inFieldLabels({
			fadeOpacity:"0.2",
			fadeDuration:"100"			
		});


//carousel
	if($('#frontpagecarousel')){

	  $('#frontpagecarousel').jcarousel({
	     vertical: false, //
	     scroll: 1, //amount of items to scroll by
	     animation: "slow", // slow - fast
	     auto: "0", //autoscroll in secunds
	     wrap: "last"
	   });

	}

	if($('#event-similar')){
    $('#event-similar').jcarousel({
       vertical: false, //
       scroll: 1, //amount of items to scroll by
       animation: "slow", // slow - fast
       auto: "0", //autoscroll in secunds
       wrap: "last"
     });
	}



});


