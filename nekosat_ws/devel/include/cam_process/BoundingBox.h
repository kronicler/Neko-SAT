// Generated by gencpp from file cam_process/BoundingBox.msg
// DO NOT EDIT!


#ifndef CAM_PROCESS_MESSAGE_BOUNDINGBOX_H
#define CAM_PROCESS_MESSAGE_BOUNDINGBOX_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace cam_process
{
template <class ContainerAllocator>
struct BoundingBox_
{
  typedef BoundingBox_<ContainerAllocator> Type;

  BoundingBox_()
    : x(0)
    , y(0)
    , height(0)
    , width(0)
    , source_id()  {
    }
  BoundingBox_(const ContainerAllocator& _alloc)
    : x(0)
    , y(0)
    , height(0)
    , width(0)
    , source_id(_alloc)  {
  (void)_alloc;
    }



   typedef int32_t _x_type;
  _x_type x;

   typedef int32_t _y_type;
  _y_type y;

   typedef int32_t _height_type;
  _height_type height;

   typedef int32_t _width_type;
  _width_type width;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _source_id_type;
  _source_id_type source_id;





  typedef boost::shared_ptr< ::cam_process::BoundingBox_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::cam_process::BoundingBox_<ContainerAllocator> const> ConstPtr;

}; // struct BoundingBox_

typedef ::cam_process::BoundingBox_<std::allocator<void> > BoundingBox;

typedef boost::shared_ptr< ::cam_process::BoundingBox > BoundingBoxPtr;
typedef boost::shared_ptr< ::cam_process::BoundingBox const> BoundingBoxConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::cam_process::BoundingBox_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::cam_process::BoundingBox_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace cam_process

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'sensor_msgs': ['/opt/ros/kinetic/share/sensor_msgs/cmake/../msg'], 'cam_process': ['/home/ttg/catkin_ws/src/cam_process/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::cam_process::BoundingBox_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cam_process::BoundingBox_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cam_process::BoundingBox_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cam_process::BoundingBox_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cam_process::BoundingBox_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cam_process::BoundingBox_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::cam_process::BoundingBox_<ContainerAllocator> >
{
  static const char* value()
  {
    return "c81602187a4973f14142d0df5a73a48d";
  }

  static const char* value(const ::cam_process::BoundingBox_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xc81602187a4973f1ULL;
  static const uint64_t static_value2 = 0x4142d0df5a73a48dULL;
};

template<class ContainerAllocator>
struct DataType< ::cam_process::BoundingBox_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cam_process/BoundingBox";
  }

  static const char* value(const ::cam_process::BoundingBox_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::cam_process::BoundingBox_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int32 x\n\
int32 y\n\
int32 height\n\
int32 width\n\
string source_id\n\
";
  }

  static const char* value(const ::cam_process::BoundingBox_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::cam_process::BoundingBox_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.x);
      stream.next(m.y);
      stream.next(m.height);
      stream.next(m.width);
      stream.next(m.source_id);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct BoundingBox_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::cam_process::BoundingBox_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::cam_process::BoundingBox_<ContainerAllocator>& v)
  {
    s << indent << "x: ";
    Printer<int32_t>::stream(s, indent + "  ", v.x);
    s << indent << "y: ";
    Printer<int32_t>::stream(s, indent + "  ", v.y);
    s << indent << "height: ";
    Printer<int32_t>::stream(s, indent + "  ", v.height);
    s << indent << "width: ";
    Printer<int32_t>::stream(s, indent + "  ", v.width);
    s << indent << "source_id: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.source_id);
  }
};

} // namespace message_operations
} // namespace ros

#endif // CAM_PROCESS_MESSAGE_BOUNDINGBOX_H