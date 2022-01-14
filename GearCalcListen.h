// Gear	lcListen.h : Include file for standard system include files,
// or project specific include files.

#pragma once


// TODO: Reference additional headers your program requires here.
struct forza_data_packet {
	int is_race_on;
	int time_stamp;
	float max_rpm;
	float idle_rpm;
	float current_rpm;
	//Acceleration in 3 axes
	float x_accel;
	float y_accel;
	float z_accel;
	//Velocity in 3 axes
	float x_velocity;
	float y_velocity;
	float z_velocity;
	//Angular Velocity in 3 axes
	float x_ang_velocity;
	float y_ang_velocity;
	float z_ang_velocity;
	//Rotation
	float yaw;
	float pitch;
	float roll;

	float suspension_travel_front_left;
	float suspension_travel_front_right;
	float suspension_travel_rear_left;
	float suspension_travel_rear_right;

	float tire_slip_ratio_front_left;
	float tire_slip_ratio_front_right;
	float tire_slip_ratio_rear_left;
	float tire_slip_ratio_rear_right;

	float wheel_speed_front_left;
	float wheel_speed_front_right;
	float wheel_speed_rear_left;
	float wheel_speed_rear_right;

	float wheel_rumblestrip_speed_front_left;
	float wheel_rumblestrip_speed_front_right;
	float wheel_rumblestrip_speed_rear_left;
	float wheel_rumblestrip_speed_rear_right;

	//WHEEL IN PUDDLE DEPTH//
	float wheel_in_puddle_depth_front_left;

	float wheel_in_puddle_depth_front_right;

	float wheel_in_puddle_depth_rear_left;

	float wheel_in_puddle_depth_rear_right;

	//SURFACE RUMBLE//
	float surface_rumble_front_left;

	float surface_rumble_front_right;
	float surface_rumble_rear_left;

	float surface_rumble_rear_right;

	//TIRE SLIP ANGLE//
	float tire_slip_angle_front_left;

	float tire_slip_angle_front_right;
	float tire_slip_angle_rear_left;

	float tire_slip_angle_rear_right;

	//TIRE COMBINED SLIP//
	float tire_combined_slip_front_left;
	float tire_combined_slip_front_right;

	float tire_combined_slip_rear_left;

	float tire_combined_slip_rear_right;

	//SUSPENSION TRAVEL METERS//
	float suspension_travel_meters_front_left;

	float suspension_travel_meters_front_right;

	float suspension_travel_meters_rear_left;

	float suspension_travel_meters_rear_right;

	//VEHICLE STATS//
	int car_ordinal;

	int car_class;

	int car_pi;

	int drivetrain;

	int num_cylinders;

	int car_category;
	int unknown_int1;

	int unknown_int2;
	//POSITION AXIS//
	float x_position;
	float y_position;
	float z_position;

	//VEHICLE ENGINE STATS//
	float engine_speed;

	float engine_power;
	float engine_torque;

	//TIRE TEMP//
	float tire_temp_front_left;

	float tire_temp_front_right;

	float tire_temp_rear_left;

	float tire_temp_rear_right;

	//RACE STATS//
	float boost;

	float fuel;

	float distance_traveled;

	float best_lap;

	float last_lap;

	float current_lap;

	float current_race_time;

	short lap_num;

	char race_position;

	//MISC//
	char accelerator_position;

	char brake_position;

	char clutch_position;

	char handbrake_position;

	char gear;

	char steering_position;

	short driving_line;

	char ai_brake_difference;


};
