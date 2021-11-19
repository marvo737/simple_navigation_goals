#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <random>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "simple_navigation_goals");

  // tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  float MAX_val = 2.0;
  float MIN_val = -2.0;

  // wait for the action server to come up
  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  while (true)
  {
    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    // Generated random value
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(MIN_val, MAX_val);

    goal.target_pose.pose.position.x = distr(eng);
    goal.target_pose.pose.position.y = distr(eng);
    goal.target_pose.pose.orientation.w = 1.0;

    ROS_INFO("target_x:%f, target_y:%f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    ac.waitForResult();

    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      ROS_INFO("The robot has arrived at its destination.");
    }
    else
    {
      ROS_INFO("Movement failed.");
    }
  }
  return 0;
}
