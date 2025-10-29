# 🚗 SEAV-II — ROS 2 Ackermann Robot (Real-World + RViz)

Autonomous Ackermann-steered robot using **HC-89 rotary encoders** (rear = wheel odometry, front = steering angle) and a **2D LiDAR** for SLAM and Nav2-based autonomous navigation.

---

## 🔧 Features
- **Ackermann drive** implemented via `ros2_control` (`acker_cont`)
- **Odometry** from rear HC-89 encoder  
- **Steering angle** from front HC-89 encoder  
- **2D LiDAR** for mapping and obstacle detection  
- **SLAM Toolbox** + **Nav2** for live mapping and goal-based navigation  
- **Digital Twin** visualization in RViz (showing TFs, odom, scan data and path)

---

## 🧩 Hardware Setup
- Raspberry Pi (ROS 2 Humble)
- Arduino (motor + encoder interface via serial)
- ZKBM1 motor driver (or equivalent)
- 2 × **HC-89 encoders** (front steering / rear odometry)
- 2D LiDAR (e.g., RPLIDAR A1/A2 or Hokuyo)

---

## 🧠 Software Stack
- ROS 2 **Humble Hawksbill**
- Core packages:  
  `ros2_control`, `controller_manager`, `twist_mux`, `slam_toolbox`, `nav2_bringup`, `tf2_ros`, `robot_localization`
- Custom packages:  
  `seav2`, `seav2_imu` (encoder + LiDAR drivers)

---

## ⚙️ Build and Install
```bash
# Clone (switch to your branch)
git clone https://github.com/se-buw/seav-ii.git
cd seav-ii

# Install dependencies
sudo apt update
sudo apt install -y \
  ros-humble-ros2-control ros-humble-controller-manager \
  ros-humble-slam-toolbox ros-humble-nav2-bringup \
  ros-humble-twist-mux ros-humble-robot-localization \
  ros-humble-tf2-ros

# Build the workspace
colcon build --symlink-install
source install/setup.bash


▶️ Run on the Robot

# 1️⃣ Launch the full robot stack (URDF + TF + LiDAR + Encoders + EKF)
ros2 launch seav2 launch_robot.launch.py

#This starts:

1)Twist Mux → Stamper → /acker_cont/reference

2)ros2_control controllers: acker_cont, joint_broad

3)Static TFs: base_link → laser_frame

4)Encoder & LiDAR drivers

5)EKF (robot_localization) fusing /acker_cont/odometry


🗺️ SLAM (Online Mapping)

# Run SLAM Toolbox with RViz
ros2 launch slam_toolbox online_async_launch.py use_sim_time:=false


🤖 Navigation (Nav2)

# After a map is built (or using online SLAM)
ros2 launch nav2_bringup bringup_launch.py use_sim_time:=false
# Then send a 2D Goal in RViz



🧠 Project Overview

This project demonstrates a ROS 2-based autonomous Ackermann-drive robot integrating:

  Real-time odometry from HC-89 encoders

  SLAM Toolbox for map generation

  Nav2 for autonomous goal navigation

  Real-world motion mirrored in RViz as a digital twin

🪪 License

  MIT License (please add a LICENSE file if not present)

🎥 Demo Preview

  Mapping: SLAM creates live 2D occupancy grid in RViz.

  Navigation: Nav2 guides the robot toward a goal.

  Execution: Real Ackermann robot follows planned path using HC-89 encoder feedback and LiDAR data.
