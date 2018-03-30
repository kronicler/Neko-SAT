//This  configures the server upon which ros is running
var ros = new ROSLIB.Ros({
    url : 'ws://'+window.location.hostname+':9090'
});
var _vid = 0; 
var active_plugins = []
var topic_view = new Vue({
    el: "#topic_view",
    data: {
      topic_list : [],
      type_list: []
    },
    methods: {
        launch: function(e) {
            console.log("Trying to launch "+e.currentTarget.innerHTML);
            index = this.topic_list.indexOf(e.currentTarget.innerHTML);
            try {
                element = new plugins[this.type_list[index]]("w"+_vid);
                document.getElementById("mission_display").innerHTML+= element.ui_template();
                element.load(ros,e.currentTarget.innerHTML);
                active_plugins.push(element);    
                _vid++;
            }
            catch(e) {
                console.log(e);
                alert("No plugin that handles the type "+this.type_list[index]+" was found."); 
            }
        }
    }
});

topic_list = {};

topic_client = new ROSLIB.Service({"ros":ros, "name":"/rosapi/topics", "serviceType":"rosapi/Topics"});
setInterval(function(){topic_client.callService({}, function(result){
    for(i = 0; i < result.topics.length; i++){
       topic_list[result.topics[i]] = result.types[i];
    }
    update_list();
})},3000);

/*
service_client = new ROSLIB.Service({"ros":ros, "name":"/rosapi/services", "serviceType":"rosapi/Services"})
setInterval(function(){
    service_client.callService({},function(result){
        result.services.forEach(function(element){
            element = String(element);
            if(element != undefined){
               if(!is_in(element)){
                   topic_list.push(element);
                   type_list.push("/services"+element);
               }
            }
        }); 
        update_list();
    });
},3000);*/
searchbar = document.getElementById("search");
searchbar.onkeyup = update_list();

function update_list(){
    tmp_list = [], typ_list = [];
    for(topic_name in topic_list){
        try{
            if(topic_name.includes(searchbar.value)){
                tmp_list.push(topic_name);
                typ_list.push(topic_list[topic_name]);
            }
        } catch(e) {
            console.log("Found error")
        }

    }
    topic_view.topic_list = tmp_list;
    topic_view.type_list = typ_list; 
}
function http_get(theUrl)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", theUrl); // false for synchronous request
    xmlHttp.setRequestHeader('Access-Control-Allow-Origin', 'http://localhost');
    xmlHttp.setRequestHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    xmlHttp.setRequestHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    xmlHttp.setRequestHeader('Access-Control-Allow-Credentials', true);
    xmlHttp.send(); 
    return xmlHttp.responseText;
}
function start_recording(){
    http_get("http://"+window.location.hostname+":5000/start")
}

function stop_recording(){
    http_get("http://"+window.location.hostname+":5000/stop")
}

function refresh_page(){
    http_get("http://"+window.location.hostname+":5000/refresh")
}

setInterval(function() {
    window.location.reload();
}, 300000); 

function manual_refresh(){
    window.location.reload();
}