$(document).ready(function(){
    $("#signin").submit(function(e)
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
                if(data.success) {
                    console.log(data.message);
                    $.cookie(data.cookie.name, data.cookie.value);
                    setTimeout(function () {
                       window.location.href = "/"; 
                    }, 1000);
                } else {
                    console.log(data.message); 
                }
            },
            error: function(jqXHR, textStatus, errorThrown) 
            {
                console.log("jqXHR: " + jqXHR.status + "\ntextStatus: " + textStatus + "\nerrorThrown: " + errorThrown);    
            }
        });
    });
});