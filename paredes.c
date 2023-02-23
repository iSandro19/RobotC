//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Wall following
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

///////////////////////////////// Definitions
const int CORNER_ANGLE = 90;
const int ADJUST_ANGLE = 10;
const int MAX_DISTANCE = 30;
const int MIN_DISTANCE = 15;

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
	return;
}

// Turn right
void right(int degrees) {
	resetGyro(S2);
    while (getGyroDegrees(S2) < degrees) {
		setMotorSpeed(motorB, 20);
		setMotorSpeed(motorC, -20);
	}
	return;
}

void perpendicular() {
	// Check if the robot is perpendicular to the detected wall in front
	// If it is no, position it perpendicular to the wall
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

// Stop
void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
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
			left(CORNER_ANGLE-5);
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
