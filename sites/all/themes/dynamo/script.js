$(document).ready(function() {
    $("#user-login-form label").overlabel();
   	$("#search-form label").overlabel();
   	$("#search-block-form label").overlabel();
//   	$("#comment-form label").overlabel();

    $('#frontpagecarousel').jcarousel({
       vertical: false, //
       scroll: 1, //amount of items to scroll by
       animation: "slow", // slow - fast
       auto: "0", //autoscroll in secunds
       wrap: "last"
     });

    $('#event-similar').jcarousel({
       vertical: false, //
       scroll: 1, //amount of items to scroll by
       animation: "slow", // slow - fast
       auto: "0", //autoscroll in secunds
       wrap: "last"
     });




});


