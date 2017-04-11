#pragma once

// Grid Map
#include <grid_map_ros/grid_map_ros.hpp>

// Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <geometry_msgs/PoseStamped.h>

#include <dynamic_reconfigure/server.h>
#include <map_combiner/MapCombinerConfig.h>


// Kindr
//#include <kindr/poses/PoseEigen.hpp>
//#include <kindr/phys_quant/PhysicalQuantitiesEigen.hpp>

// ROS (time)
#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <hector_worldmodel_msgs/ObjectModel.h>



#include <cv_debug_provider/cv_debug_provider.h>

#include <grid_map_core/Polygon.hpp>



namespace map_combiner {

/*!
 * Combines grid map data from a premade static map and a rolling elevation map
 */
class MapCombiner
{
 public:

  /*!
   * Constructor.
   */
  MapCombiner();


  bool combineMaps();

protected:
  void staticMapCallback(const nav_msgs::OccupancyGrid& msg);
  void localElevationMapCallback(const grid_map_msgs::GridMapConstPtr& msg);
  void worldmodelCallback(const hector_worldmodel_msgs::ObjectModel& msg);

  void poseCallback(const geometry_msgs::PoseStampedConstPtr& msg);
  void collisionPoseCallback(const geometry_msgs::PoseStampedConstPtr& msg);

  void initialPoseCallback(const geometry_msgs::PoseWithCovarianceStamped pose);

  void dynRecParamCallback(map_combiner::MapCombinerConfig &config, uint32_t level);

  void segmentObstacleAt(const geometry_msgs::Pose& pose, const double search_area_edge_length);
  void segmentObstacleAt(const grid_map::Position& pos, const double search_area_edge_length);

  void publishFusedNavGrid();
  void callElevationMapReset();
  void callMasonMapReset();

  bool updateInflatedLayer(grid_map::GridMap& map);

  ros::Subscriber static_map_sub_;
  ros::Subscriber local_elevation_map_sub_;
  ros::Subscriber worldmodel_sub_;

  ros::Subscriber pose_sub_;
  ros::Subscriber collision_pose_sub_;
  ros::Subscriber initial_pose_sub_;

  ros::Publisher fused_map_pub_;
  ros::Publisher fused_ros_map_pub_;

  //ros::Publisher debug_map_pub_;

  ros::Publisher pose_percept_publisher_;

  ros::Publisher obstacle_marker_pub_;
  visualization_msgs::Marker obstacle_marker_box_;
  visualization_msgs::Marker obstacle_marker_text_;


  ros::ServiceClient reset_elev_map_service_client_;


  grid_map::GridMap static_map_retrieved_;
  grid_map::GridMap static_map_fused_;

  grid_map::Polygon footprint_poly_;

  //! Fused elevation map as grid map.
  grid_map::GridMap local_elevation_map_;

  geometry_msgs::PoseStampedConstPtr robot_pose_;

  double p_obstacle_diff_threshold_;
  double p_pose_height_offset_;
  double p_footprint_x;
  double p_footprint_y;
  double p_large_obstacle_radius_;
  double p_small_obstacle_radius_;
  bool p_publish_percept_;
  bool p_fuse_elevation_map_;

  dynamic_reconfigure::Server<map_combiner::MapCombinerConfig> dyn_rec_server_;

  boost::shared_ptr<CvDebugProvider> debug_img_provider_;
  boost::shared_ptr<CvDebugProvider> flood_debug_img_provider_;
  ros::Publisher poly_debug_pub_;
  ros::Publisher obstacle_poly_pub_;
  ros::Publisher mason_reset_pub_;
};

} /* namespace */
