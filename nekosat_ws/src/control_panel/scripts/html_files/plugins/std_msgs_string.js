class string_handler{
    constructor(view_id){
        this.template = {}
        this.view_id = view_id;
    }

    ui_template() {
       return "<div id=\""+this.view_id+"\"class=\"col-sm-4 card\"><div class=\"card-body\">"+
              "<h4 class=\"card-title\">{{text}}</h4><p class=\"card-text\">{{topic}}</p>"+
              "</div></div>";
    }

    load(ros_handle, topic_name){
        this.template = new Vue({
          el: "#"+this.view_id,
          data: {
              text: "Loading...",
              topic: topic_name
          }
        });
        this.listener = new ROSLIB.Topic({
            "ros": ros_handle,
            "name": topic_name,
            "messageTpye": 'std_msgs/String'
        });
        self = this;
        this.listener.subscribe(function(msg){
		self.template.text=msg.data
        });
    }
};
