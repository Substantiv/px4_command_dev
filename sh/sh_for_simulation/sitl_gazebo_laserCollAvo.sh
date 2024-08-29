##sitl_gazebo
gnome-terminal --window -e 'bash -c "roscore; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch px4 posix_sitl.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch mavros px4.launch fcu_url:="udp://:14540@127.0.0.1:14557"; exec bash"' \
--tab -e 'bash -c "sleep 3; roslaunch px4_command collision_avoidance.launch; exec bash"' \
--tab -e 'bash -c "sleep 3; rosrun px4_command set_mode; exec bash"' \

