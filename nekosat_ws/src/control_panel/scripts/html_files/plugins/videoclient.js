//TODO change hardcoded url
class video_handler {
    constructor(view_id){
        this.template = {}
        this.view_id = view_id;
    }

    ui_template() {
       return "<div id=\""+this.view_id+"\"class=\"col-sm-6 card\"><div class=\"card-body\">"+
              "<img :src=\"image_source\"></img>"+
              "<p class=\"card-text\">{{topic}}</p>"+
              "</div></div>";
    }

    load(ros_handle, topic_name){
        this.template = new Vue({
          el: "#"+this.view_id,
          data: {
              topic: topic_name,
              image_source: "http://"+window.location.hostname+":8080/stream?topic="+topic_name
          }
        }); 
    }
};

class compressed_video_handler {
    constructor(view_id){
        this.template = {}
        this.view_id = view_id;
    }

    ui_template() {
       return "<div id=\""+this.view_id+"\"class=\"col-sm-6 card\"><div class=\"card-body\">"+
              "<img :src=\"image_source\"></img>"+
              "<p class=\"card-text\">{{topic}}</p>"+
              "</div></div>";
    }

    load(ros_handle, topic_name){

        this.template = new Vue({
          el: "#"+this.view_id,
          data: {
              topic: topic_name,
              image_source: "http://"+window.location.hostname+":8080/stream?topic="+topic_name.slice(0, -"/compressed".length)
+                            + "&type=ros_compressed"
          }
        });                                  
    }
};

