
#include "controller.h"



void Debug_Controller_State (Controller_State* state) {
  printf("DEBUG_CONTROLLER_STATE:\t");
  printf("rover_state=");
  switch(state->rover_state){
    case NOMINAL : {
      printf("NOMINAL\t");
      break;
    }
    case OBSTACLE : {
      printf("OBSTACLE\t");
      break;
    }
    case RETURN : {
      printf("RETURN\t");
      break;
    }
  }
  printf("time_step=%f\t", state->time_step);
  printf("\n");
}
void Debug_Controller_Input (Controller_Input* input) {
  printf("DEBUG_CONTROLLER_INPUT:\t");
  printf("emergency_button=%f\t", input->battery);
  printf("battery=%s\t", input->emergency_button?"true":"false");
  printf("radar_sensors=[%f", input->radar_sensors[0]);
  for (int i=1; i<ROVER_NUMBER_RADAR_SENSOR; i++) {
    printf(",%f", input->radar_sensors[i]);
  }
  printf("]\t");
  printf("\n");
}
void Debug_Controller_Output (Controller_Output* output) {
  printf("DEBUG_CONTROLLER_OUTPUT:\t");
  printf("speed_wheels=[%f", output->speed_wheels[0]);
  for (int i=1; i<ROVER_NUMBER_WHEEL; i++) {
    printf(",%f", output->speed_wheels[i]);
  }
  printf("]\t");
  printf("\n");
}



bool Controller_detectObstacle (Controller_Input* input) {
  for (int i=0; i<ROVER_NUMBER_RADAR_SENSOR; i++) {
    if (input -> radar_sensors[i] < CONTROLLER_SAFE_ZONE_RAY) {
      return true;
    }
  }
  return false;
}
bool Controller_isLowBattery (Controller_Input* input) {
  return input -> battery < 10;
}
bool Controller_isEmergencyButtonPressed (Controller_Input* input) {
  return input -> emergency_button;
}



void Controller_toNominalState (Controller_State* state) {
  state->rover_state = NOMINAL;
  state->time_step = CONTROLLER_DEFAULT_TIME_STEP;
}
void Controller_toObstacleState (Controller_State* state) {
  state->rover_state = OBSTACLE;
  state->time_step = CONTROLLER_DEFAULT_TIME_STEP/2;
}
void Controller_toReturnState (Controller_State* state) {
  state->rover_state = RETURN;
  state->time_step = CONTROLLER_DEFAULT_TIME_STEP;
}



void Controller_init (Controller_State* state) {
  state -> rover_state = NOMINAL;
  state -> time_step = CONTROLLER_DEFAULT_TIME_STEP;
}

void Controller_step (
  Controller_State* state, 
  Controller_Input* input, 
  Controller_Output* output
) {
  switch(state->rover_state) {
    case NOMINAL : {
      if (
        Controller_isLowBattery(input) ||
        Controller_isEmergencyButtonPressed(input)
      ){
        Controller_toReturnState(state);
        break;
      }
      if (Controller_detectObstacle(input)){
        Controller_toObstacleState(state);
        break;
      }
      break;
    }
    case OBSTACLE : {
      if (
        Controller_isLowBattery(input) ||
        Controller_isEmergencyButtonPressed(input)
      ){
        Controller_toReturnState(state);
        break;
      }
      if (!Controller_detectObstacle(input)){
        Controller_toNominalState(state);
        break;
      }
      break;
    }
    case RETURN : {
      break;
    }
    default : {
      break;
    }
  }

  switch(state->rover_state) {
    case NOMINAL : {
      for (int i=0; i<ROVER_NUMBER_WHEEL; i++) {
        output->speed_wheels[i] = ROVER_DEFAULT_SPEED;
      }
      break;
    }
    case OBSTACLE : {
      for (int i=0; i<ROVER_NUMBER_WHEEL; i++) {
        output->speed_wheels[i] = -0.5 * ROVER_DEFAULT_SPEED;
      }
      break;
    }
    case RETURN : {
      for (int i=0; i<ROVER_NUMBER_WHEEL/2; i++) {
        output->speed_wheels[2*i] = ROVER_DEFAULT_SPEED;
        output->speed_wheels[2*i+1] = -1.0 * ROVER_DEFAULT_SPEED;
      }
      break;
    }
    default : {
      break;
    }
  }

  usleep(1000 * state->time_step);
}

void Controller_loop () {
  Controller_State state;
  Controller_init(&state);
  Controller_Input input = {
    .emergency_button = false,
    .battery = 100.0,
    .radar_sensors = {
      200.0,
      200.0,
      200.0,
      200.0,
      200.0
    }
  };
  Controller_Output output;

  int cnt=0;
  while (cnt < 100) {
    printf("cnt=%d\n", cnt++);
    Debug_Controller_Input(&input);
    Debug_Controller_State(&state);
    Controller_step(&state, &input, &output);
    Debug_Controller_State(&state);
    Debug_Controller_Output(&output);
    printf("\n");
  }
}



void Controller_tests () {
  int cnt = 0;
  bool flag;

  if (true) {
    Controller_State state = {
      .rover_state = NOMINAL,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 100.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == NOMINAL;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = NOMINAL,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 100.0,
      .radar_sensors = {
        50.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == OBSTACLE;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = NOMINAL,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 5.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = NOMINAL,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = true,
      .battery = 100.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }



  if (true) {
    Controller_State state = {
      .rover_state = OBSTACLE,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 100.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == NOMINAL;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = OBSTACLE,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 100.0,
      .radar_sensors = {
        50.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == OBSTACLE;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = OBSTACLE,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 5.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = OBSTACLE,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = true,
      .battery = 100.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }



  if (true) {
    Controller_State state = {
      .rover_state = RETURN,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 100.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = RETURN,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 100.0,
      .radar_sensors = {
        50.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = RETURN,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = false,
      .battery = 5.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }

  if (true) {
    Controller_State state = {
      .rover_state = RETURN,
      .time_step = ROVER_DEFAULT_SPEED
    };
    Controller_Input input = {
      .emergency_button = true,
      .battery = 100.0,
      .radar_sensors = {
        200.0,
        200.0,
        200.0,
        200.0,
        200.0
      }
    };
    Controller_Output output;
    Controller_step(&state, &input, &output);
    flag = state.rover_state == RETURN;
    printf("Test %d : %s \n", cnt++, flag?"Success":"Failure");
  }
}


/*
int main () {
  //Controller_loop();
  return 0;
}
*/