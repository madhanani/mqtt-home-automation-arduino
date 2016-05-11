$(document).ready(function(){
    $.get( "/getsubgpiodata", function(data) {
        console.log(data);
        $.each(data, function(key, value) {
            $("#" + key).val(value);
        });
    });

    $("#subgpio").submit(function(e)
    {
        e.preventDefault(); 
        var postData = $(this).serialize();
        console.log(postData);
        var formURL = $(this).attr("action");
        $.ajax(
        {
            url : formURL,
            type: 'POST',
            data : postData,
            dataType: 'json',
            success:function(data, textStatus, jqXHR) 
            {
                console.log(data);
            },
            error: function(jqXHR, textStatus, errorThrown) 
            {
                console.log("jqXHR: " + jqXHR.status + "\ntextStatus: " + textStatus + "\nerrorThrown: " + errorThrown);    
            }
        });
    });

    $("#logout").on('click', function(e)
    {
        e.preventDefault(); 
        var cookieName = $("#cookieName").val();
        $.cookie(cookieName, null);
        setTimeout(function () {
           window.location.href = "/"; 
        }, 1000);
    });

});