var keyStates = {
    "w": false,
    "a": false,
    "s": false,
    "d": false,
    " ": false
};

function callback2(text) {
    // alert(text);
}

function httpPostAsync(theUrl, data, callback)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.response);
    }
    xmlHttp.open("POST", theUrl, true); // true for asynchronous 
    xmlHttp.send(data);
}

function httpGetAsync2(theUrl, callback)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", theUrl, true); // true for asynchronous 
    xmlHttp.send(null);
}

function setState(key, toSet){
    if(keyStates[key] !== toSet){
        keyStates[key] = toSet;
        control = JSON.stringify(keyStates) + "\r\n";
        httpPostAsync("/data", control, callback2); 
        // httpGetAsync2("/data", callback2); 
    }
}

function handleEvent(event, toSet){
    if(event.key === "w" || event.key === "ArrowUp"){
        setState("w", toSet);
    }else if(event.key === "a" || event.key === "ArrowLeft"){
        setState("a", toSet);
    }else if(event.key === "s" || event.key === "ArrowDown"){
        setState("s", toSet);
    }else if(event.key === "d" || event.key === "ArrowRight"){
        setState("d", toSet);
    }else if(event.key === " ") {
        setState(" ", toSet);
    }
}

document.addEventListener("keydown", function (event) {
    handleEvent(event, true);
});

document.addEventListener("keyup", function (event) {
    handleEvent(event, false);
});
