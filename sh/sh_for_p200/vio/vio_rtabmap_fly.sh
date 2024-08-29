##激光雷达自主飞行脚本
gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 5; roslaunch mavros px4.launch fcu_url:="/dev/ttyTHS2:921600"; exec bash"' \
--tab -e 'bash -c "sleep 10; roslaunch realsense2_camera rs_d400_and_t265_2.launch; exec bash"' \
--tab -e 'bash -c "sleep 20; roslaunch realsense2_camera rs_d400_and_t265_1.launch; exec bash"' \
--tab -e 'bash -c "sleep 25; roslaunch realsense2_camera rs_rtabmap.launch; exec bash"' \
--tab -e 'bash -c "sleep 30; roslaunch vision_to_mavros t265_tf_to_mavros_rtabmap.launch; exec bash"' \
--tab -e 'bash -c "sleep 35; roslaunch px4_command px4_pos_estimator.launch; exec bash"' \
--tab -e 'bash -c "sleep 40; roslaunch px4_command px4_pos_controller.launch; exec bash"' \
--tab -e 'bash -c "sleep 40; rosrun px4_command move; exec bash"' \

