$(document).ready(function(){
    $("#captchaButton").click(function(){
        $(this).text("验证中...");
        setTimeout(() => {
            $(this).text("验证成功！");
            $(this).css('background-image', 'linear-gradient(to right, #00b09b, #96c93d)');
        }, 1500); // 模拟验证过程
    });
});
