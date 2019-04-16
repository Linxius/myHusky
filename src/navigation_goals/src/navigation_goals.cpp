#include<ros/ros.h>
#include<move_base_msgs/MoveBaseAction.h>
#include<actionlib/client/simple_action_client.h>
#include<cstdlib>
#include<ctime>
#include<cstdio>

bool region = 1;
bool region_circle = 0;
double radius;
double side_length;
double posX, posY,  posW;
double posZ = 0;
int rand_count;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "navigation_goals");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //we'll send a goal to the robot to move 1 meter forward
  //goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.frame_id = "odom";
  goal.target_pose.header.stamp = ros::Time::now();

  ros::param::get("~posW", posW);
  goal.target_pose.pose.position.z = posZ;
  goal.target_pose.pose.orientation.w = posW;

  ros::param::get("~region", region);

  if( !region ){
    ros::param::get("~posX", posX);
    ros::param::get("~posY", posY);
    goal.target_pose.pose.position.x = posX;
    goal.target_pose.pose.position.y = posY;

    ROS_INFO("Sending goal");
    ac.sendGoal(goal);
    ac.waitForResult();

    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("move SUCCEEDED");
    else
      ROS_INFO("move FAILED");
    return 0;
  }

  ros::param::get("~side_length", side_length);
  double randx, randy;
  ros::param::get("~rand_count", rand_count);
  for( ; rand_count; rand_count--){
    srand( time(0) );
    ros::param::get("~region_circle", region_circle);
    if( !region_circle ){
      ros::param::get("side_length", side_length);
      // goal.target_pose.pose.position.x = side_length * (double)rand() / (double)RAND_MAX;
      // goal.target_pose.pose.position.y = side_length * (double)rand() / (double)RAND_MAX;
      randx = side_length * rand() / RAND_MAX - side_length / 2;
      randy = side_length * rand() / RAND_MAX - side_length / 2;
      printf("randx = %f\n", randx);
      printf("randy = %f\n", randy);
      printf("count = %d\n", rand_count);
      goal.target_pose.pose.position.x = randx;
      goal.target_pose.pose.position.y = randy;
    }else{
      ros::param::get("~radius", radius);
    }
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);
    ac.waitForResult();

    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("move SUCCEEDED");
    else
      ROS_INFO("move FAILED");
  }

  return 0;
}
