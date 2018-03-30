# Hornet control panel

This package is used for debuging our vehicle. It relies on ROS Web tools for controls. The scripts folder contains a python server and the associated HTML files. In order for this to run simply run

```
roslaunch control_panel server.launch
```

Each message topic is listed in the side bar of the control panel. To handle a new topic create a new plugin inside the scripts/html_files/plugins folder.

