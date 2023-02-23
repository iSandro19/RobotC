//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Wall following
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

///////////////////////////////// Definitions
const int CORNER_ANGLE = 90;
const int ADJUST_ANGLE = 15;
const int MAX_DISTANCE = 30;
const int MIN_DISTANCE = 15;

// Stop
void stop() {
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	return;
}

// Forward
void forward(int speed) {
	setMotorSpeed(motorB, speed);
	setMotorSpeed(motorC, speed);
    return;
}

// Reverse
void reverse(int speed) {
    setMotorSpeed(motorB, -speed);
    setMotorSpeed(motorC, -speed);
    return;
}

// Turn left
void left(int degrees) {
	resetGyro(S2);
    while (getGyroDegrees(S2) > -degrees) {
		setMotorSpeed(motorB, -20);
		setMotorSpeed(motorC, 20);
	}
	stop();
	return;
}

// Turn right
void right(int degrees) {
	resetGyro(S2);
    while (getGyroDegrees(S2) < degrees) {
		setMotorSpeed(motorB, 20);
		setMotorSpeed(motorC, -20);
	}
	stop();
	return;
}

// Perpendicular
void perpendicular() {
	// If robot is perpendicular, then its distance is smaller than small left turn distance and small right turn distante
	// If it is not perpendicular, move to the position
	int front_distance = 0;
	int left_distante = 0;
	int right_distante = 0;

	// Check front wall
	front_distance = getUSDistance(S4);
	writeDebugStreamLine("Front: %d", front_distance);

	// Check left wall
	left(ADJUST_ANGLE);
	left_distante = getUSDistance(S4);
	writeDebugStreamLine("Left: %d", left_distante);
	right(ADJUST_ANGLE);

	// Check right wall
	right(ADJUST_ANGLE);
	right_distante = getUSDistance(S4);
	writeDebugStreamLine("Right: %d", right_distante);
	left(ADJUST_ANGLE);

	if(front_distance == left_distante &&
	   front_distance == right_distante) {
		writeDebugStreamLine("It is perpendicular");
		return;
	}

	// If robot is not perpendicular, move to the position
	if(left_distante < front_distance) {
		writeDebugStreamLine("Adjust a little bit to the left");
		left(ADJUST_ANGLE);
	} else if (right_distante < front_distance){
		writeDebugStreamLine("Adjust a little bit to the right");
		right(ADJUST_ANGLE);
	}

	return;
}

// Check right wall
bool check_wall() {
	int distance = 0;

	right(CORNER_ANGLE);
	distance = getUSDistance(S4);
	if(distance < MAX_DISTANCE) {
		if(distance <= MIN_DISTANCE) {
			reverse(50);
			sleep(500);
		}
		return true;
	}
	else {
		return false;
	}
}
/////////////////////////////////

///////////////////////////////// Main task
task main() {
	clearDebugStream();
	bool right_wall = false;
	bool front_wall = false;

	while(true) {
		right_wall = check_wall();
		perpendicular();

		if(right_wall) {
			left(CORNER_ANGLE-4);
			clearTimer(T1);
			while(time1[T1] < 1000) {
				forward(50);
				if(getUSDistance(S4) <= MIN_DISTANCE) {
					left(CORNER_ANGLE);
					break;
				}
			}
		} else {
			front_wall = false;
			while(front_wall == false) {
				forward(50);
				if(getUSDistance(S4) < (MAX_DISTANCE+MIN_DISTANCE)/2) {
					front_wall = true;
					left(CORNER_ANGLE);
				}
			}
		}
	}
}
///////////////////////////////// End
