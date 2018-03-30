;; Auto-generated. Do not edit!


(when (boundp 'cam_process::BoundingBox)
  (if (not (find-package "CAM_PROCESS"))
    (make-package "CAM_PROCESS"))
  (shadow 'BoundingBox (find-package "CAM_PROCESS")))
(unless (find-package "CAM_PROCESS::BOUNDINGBOX")
  (make-package "CAM_PROCESS::BOUNDINGBOX"))

(in-package "ROS")
;;//! \htmlinclude BoundingBox.msg.html


(defclass cam_process::BoundingBox
  :super ros::object
  :slots (_x _y _height _width _source_id ))

(defmethod cam_process::BoundingBox
  (:init
   (&key
    ((:x __x) 0)
    ((:y __y) 0)
    ((:height __height) 0)
    ((:width __width) 0)
    ((:source_id __source_id) "")
    )
   (send-super :init)
   (setq _x (round __x))
   (setq _y (round __y))
   (setq _height (round __height))
   (setq _width (round __width))
   (setq _source_id (string __source_id))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:height
   (&optional __height)
   (if __height (setq _height __height)) _height)
  (:width
   (&optional __width)
   (if __width (setq _width __width)) _width)
  (:source_id
   (&optional __source_id)
   (if __source_id (setq _source_id __source_id)) _source_id)
  (:serialization-length
   ()
   (+
    ;; int32 _x
    4
    ;; int32 _y
    4
    ;; int32 _height
    4
    ;; int32 _width
    4
    ;; string _source_id
    4 (length _source_id)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _x
       (write-long _x s)
     ;; int32 _y
       (write-long _y s)
     ;; int32 _height
       (write-long _height s)
     ;; int32 _width
       (write-long _width s)
     ;; string _source_id
       (write-long (length _source_id) s) (princ _source_id s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _x
     (setq _x (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _y
     (setq _y (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _height
     (setq _height (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _width
     (setq _width (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _source_id
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _source_id (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get cam_process::BoundingBox :md5sum-) "c81602187a4973f14142d0df5a73a48d")
(setf (get cam_process::BoundingBox :datatype-) "cam_process/BoundingBox")
(setf (get cam_process::BoundingBox :definition-)
      "int32 x
int32 y
int32 height
int32 width
string source_id

")



(provide :cam_process/BoundingBox "c81602187a4973f14142d0df5a73a48d")


