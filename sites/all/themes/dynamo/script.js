$(document).ready(function() {
    $("#user-login-form label").overlabel();
   	$("#search-form div label").overlabel();
   	$("#search-block-form label").overlabel();

    $('#frontpagecarousel').jcarousel({
       vertical: false, //
       scroll: 1, //amount of items to scroll by
       animation: "slow", // slow - fast
       auto: "0", //autoscroll in secunds
       wrap: "last"
     });
});


