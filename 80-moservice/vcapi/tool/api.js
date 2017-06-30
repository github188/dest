function AjustContentHeight(){
    var div_content = document.getElementById("div_content");
    var div_body = document.getElementById("div_body")
    var clientHeight = document.documentElement.clientHeight;
    clientHeight -= 69;
    div_content.style.height = clientHeight + "px";
    div_body.style.height = clientHeight + "px";

}

window.onload=function(){AjustContentHeight();}
window.onresize=function(){AjustContentHeight();}