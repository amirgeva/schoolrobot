#pragma once

/////////////////////////////
// Network functions
/////////////////////////////

// Call once to setup Access point
void        setup_connection();
// Call at each loop to process incoming UDP messages
bool        handle_incoming();
// Call at will to send a message back to the last machine that sent a command
void        send_message(const char* value);

// Control functions

// Called to process incoming UDP message
const char* process_command(char* msg);

// Specific command handlers
void        process_drive_command(int* params, int n);
void        process_step_command(int* params, int n);
void        process_servo_command(int* params, int n);
void        process_lidar_command(int* params, int n);

// Call once to setup PWM controller
void        pwm_setup();
// Call each loop to monitor encodering pins
void        poll_encoders();

// Call once to setup LIDAR
void        sensor_setup();
// Call at each loop to read sensor and send result
void        poll_sensor();

