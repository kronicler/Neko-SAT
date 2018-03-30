class visualnav_rigid_transform{
    constructor(view_id){
        this.template = {}
        this.view_id = view_id;
    }

    ui_template() {
       return "<div id=\""+this.view_id+"\"class=\"col-sm-4 card\"><div class=\"card-body\">"+
              "<ul><li>Scale: {{scale}}</li><li>Translation: {{x}},{{y}}</li><li>Rotation: {{rot}}</li></ul><p class=\"card-text\">{{topic}}</p>"+
              "</div></div>";
    }

    load(ros_handle, topic_name){
        this.template = new Vue({
          el: "#"+this.view_id,
          data: {
              scale: 0,
              x: 0,
              y: 0,
              rot: 0,
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
             console.log("");
             self.template.scale=msg.scale;
             self.template.x = msg.x;
             self.template.y = msg.y;
             self.template.rot = msg.rot;
        });
    }
};
