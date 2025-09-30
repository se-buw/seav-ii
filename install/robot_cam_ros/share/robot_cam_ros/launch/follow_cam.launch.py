from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robot_cam_ros',
            executable='follow_cam',
            name='follow_cam',
            output='screen',
            parameters=[
                {'port': '/dev/ttyACM0'},
                {'baud': 57600},
                {'cam_index': 0},
                {'width': 640},
                {'height': 480},
                {'fov_deg': 60.0},
                {'center_ticks': 0},
                {'ticks_per_deg': 4.0},
                {'max_right_deg': 25.0},
                {'max_left_deg': 25.0},
                {'kp': 0.22},
                {'kd': 0.35},
                {'pix_sat': 0.45},
                {'base_tpf': 40},
                {'max_tpf': 120},
                {'min_tpf': 0},
                {'area_near': 0.22},
                {'area_far': 0.02},
                {'steer_cmd': 'f'},
                {'speed_cmd': 'm'},
                # HSV for orange; adjust as needed
                {'hlo': 5}, {'slo': 120}, {'vlo': 120},
                {'hhi': 25}, {'shi': 255}, {'vhi': 255},
            ]
        )
    ])
