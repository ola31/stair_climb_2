#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int8.h"

double angle_y = 0.0;

double linear_x = 0.0;
double angular_z = 0.0;

int phase = 1;
int phase_2_count = 1;

int operating_mode = 5;

void angle_y_Callback(const std_msgs::Float32::ConstPtr& msg)
{
    angle_y = msg->data;

}

void modeCallback(const std_msgs::Int8::ConstPtr& msg){

  if(operating_mode != msg->data){

    //ROS_INFO("modecallback1");
    operating_mode = msg->data;
  }

  //ROS_INFO("modecallback2");
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "stair_climb_2_node");
    ros::NodeHandle nh;

    ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 100);
    ros::Subscriber sub = nh.subscribe("/angle/y", 1000, angle_y_Callback);
    ros::Subscriber mode_sub = nh.subscribe("/mode", 10, modeCallback);


    ros::Rate loop_rate(10);
    while (ros::ok())
    {
       geometry_msgs::Twist vel_msg;

       if(phase == 1){
           linear_x = 0.2;
           angular_z = 0.0;
           if(angle_y > -10.0){
               phase++;
           }

        }
       else if(phase == 2){
           linear_x = 0.2;
           angular_z = 0.0;
           if(angle_y < -5.0){
               phase_2_count++;
               if(phase_2_count > 20){
                   phase ++;
               }
           }
       }
       else if(phase >=3 ){
           linear_x = 0.0;
           angular_z = 0.0;
       }

       vel_msg.linear.x = linear_x;
       vel_msg.angular.z = angular_z;

       if(operating_mode == 5){
          cmd_vel_pub.publish(vel_msg);
       }
       else{
         ROS_WARN("mode is not 5");
       }






        ros::spinOnce();

        loop_rate.sleep();
    }

    return 0;
}
