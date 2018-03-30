/*
* This File contains a list of all the 
*/
var plugins = {
    "std_msgs/String": string_handler,
    "sensor_msgs/Image": video_handler,
    "sensor_msgs/CompressedImage": compressed_video_handler,
    "visual_nav/flow_result": visualnav_rigid_transform
};

