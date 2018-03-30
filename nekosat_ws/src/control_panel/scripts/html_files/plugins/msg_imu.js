class imu_handler{
    constructor(view_id){
        this.template = {}
        this.view_id = view_id;
    }

    ui_template() {
       return "<div id=\""+this.view_id+"\"class=\"col-sm-4 card\"><div class=\"card-body\">"+
              "<ul><li>xi acceleration: {{x_value}} ms<sup>-2</sup></li>"+
              "<li>y acceleration: {{y_value}} ms<sup>-2<sup></li>"+
              "<li>orientation: {{angle}}</li>"
              "</div></div>";
    }

    load(ros_handle, topic_name){
        this.template = new Vue({
          el: "#"+this.view_id,
          data: {
              x_value: 0,
              y_value: 0,
              angle: 0
          }
        });
        this.listener = new ROSLIB.Topic({
            "ros": ros_handle,
            "name": topic_name,
            "messageTpye": 'sensor_msgs/Imu'
        });
        self = this;
        this.listener.subscribe(function(msg){
		self.template.x_value = msg.linear_acceleration.x*10;
                self.template.y_value = msg.linear_acceleration.y*10;
                self.template.angle = msg.orientation;
        });
    }
};
