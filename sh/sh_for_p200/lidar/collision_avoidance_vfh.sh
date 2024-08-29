##circle tracking
gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 5; roslaunch mavros px4.launch fcu_url:="/dev/ttyTHS2:921600"; exec bash"' \
--tab -e 'bash -c "sleep 5; roslaunch rplidar_ros rplidar.launch; exec bash"' \
--tab -e 'bash -c "sleep 5; roslaunch px4_command collision_avoidance_vfh.launch; exec bash"' \

