#ifndef CONTROLLER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>



#define ROVER_NUMBER_WHEEL 6
#define ROVER_NUMBER_RADAR_SENSOR 5
#define ROVER_DEFAULT_SPEED 2.0 // en m/s

#define CONTROLLER_DEFAULT_TIME_STEP 100 // en ms
#define CONTROLLER_SAFE_ZONE_RAY 100 // en cm



typedef enum {
  NOMINAL, OBSTACLE, RETURN
} Rover_State;

typedef struct {
  Rover_State rover_state;
  float time_step;
} Controller_State;

typedef struct {
  bool emergency_button;
  float battery;
  float radar_sensors[ROVER_NUMBER_RADAR_SENSOR];
} Controller_Input;

typedef struct {
  float speed_wheels[ROVER_NUMBER_WHEEL];
} Controller_Output;



void Debug_Controller_State (Controller_State* state);
void Debug_Controller_Input (Controller_Input* input);
void Debug_Controller_Output (Controller_Output* output);



bool Controller_detectObstacle (Controller_Input* input);
bool Controller_isLowBattery (Controller_Input* input);
bool Controller_isEmergencyButtonPressed (Controller_Input* input);



void Controller_toNominalState (Controller_State* state);
void Controller_toObstacleState (Controller_State* state);
void Controller_toReturnState (Controller_State* state);



void Controller_init (Controller_State* state);

void Controller_step (
  Controller_State* state, 
  Controller_Input* input, 
  Controller_Output* output
);

void Controller_loop ();



#endif