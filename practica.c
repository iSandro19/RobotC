//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Practice 1
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Lei

// Definitions ################################################################
const int CORNER_ANGLE = 90;
const int ADJUST_ANGLE = 5;
const int MAX_DISTANCE = 30;
const int MIN_DISTANCE = 5;

bool right_wall = false;
bool front_wall = false;
int currentDistance = 0;

// Semaphores #################################################################
TSemaphore semaphore12;
TSemaphore semaphore23;
TSemaphore semaphore34;

// Functions ##################################################################
void stop() {
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	return;
}

void forward(int speed) {
	setMotorSpeed(motorB, speed);
	setMotorSpeed(motorC, speed);
    return;
}

void reverse(int speed) {
    setMotorSpeed(motorB, -speed);
    setMotorSpeed(motorC, -speed);
    return;
}

void left(int degrees) {
    repeatUntil(getGyroDegrees(S2) > -degrees) {
		setMotorSpeed(motorB, -20);
		setMotorSpeed(motorC, 20);
	}
	stop();
	return;
}

void right(int degrees) {
    repeatUntil(getGyroDegrees(S2) < degrees) {
		setMotorSpeed(motorB, 20);
		setMotorSpeed(motorC, -20);
	}
	stop();
	return;
}

bool check_wall() {
	int distance = 0;

	right(CORNER_ANGLE);
	distance = getUSDistance(S4);
	if(distance < MAX_DISTANCE) {
		if(distance <= MIN_DISTANCE) {
			writeDebugStreamLine("Oh, too close to the wall, reverse a little");
			reverse(50);
			sleep(500);
		}
		return true;
	}
	else {
		return false;
	}
}

void perpendicular() {
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

	// If robot is perpendicular, do nothing
	if(front_distance == left_distante &&
	   front_distance == right_distante) {
		writeDebugStreamLine("It is perpendicular");
		return;
	}

	// If left_distance < front_distance, move a little to the left
	if(left_distante < front_distance) {
		writeDebugStreamLine("Adjust a little bit to the left");
		left(ADJUST_ANGLE);
		return;
	} else if(right_distante < front_distance) {
		writeDebugStreamLine("Adjust a little bit to the right");
		right(ADJUST_ANGLE);
		return;
	}
	return;
}


// Tasks ######################################################################
task escape() {
    while(true) {
		int currentDistance = getUSDistance(S4);

		if(currentDistance < MIN_DISTANCE) {
			semaphoreLock(semaphore12);
			if(bDoesTaskOwnSemaphore(semaphore12)) {
				writeDebugStreamLine("ESCAPE\n");
				reverse(50);
				sleep(500);
				resetGyro(S2);
				left(CORNER_ANGLE);
				resetGyro(S2);
				semaphoreUnlock(semaphore12);
			}
		}
	}
}

task light() {
	int luz, luzUmbral;
	luzUmbral = getColorAmbient(S3)*1.4;

	while(true) {
		luz = getColorAmbient(S3);

		semaphoreLock(semaphore12);
		if(bDoesTaskOwnSemaphore(semaphore12)) {
			writeDebugStreamLine("LIGHT\n");

			if(bDoesTaskOwnSemaphore(semaphore23)) {
				semaphoreUnlock(semaphore23);
			}

			if(luz > luzUmbral) {
				semaphoreLock(semaphore23);
				setMotorSpeed(motorB, 30);
				setMotorSpeed(motorC, 0);
				while(luz <= getColorAmbient(S3)) {
					luz = getColorAmbient(S3);
				}
				setMotorSpeed(motorB, 0);
				setMotorSpeed(motorC, 30);
				while(luz <= getColorAmbient(S3)) {
					luz = getColorAmbient(S3);
				}
				semaphoreUnlock(semaphore23);
			}
			semaphoreUnlock(semaphore12);
		} else {
			if(!bDoesTaskOwnSemaphore(semaphore23)) {
				semaphoreLock(semaphore23);
			}
		}
	}
}

task follow_wall() {
	bool right_wall = false;

	while(true) {
		int currentDistance = getUSDistance(S4);

		semaphoreLock(semaphore23);
		if(bDoesTaskOwnSemaphore(semaphore23)) {
			writeDebugStreamLine("FOLLOW WALL\n");

			if(bDoesTaskOwnSemaphore(semaphore34)) {
				semaphoreUnlock(semaphore34);
			}

			if(currentDistance < MAX_DISTANCE) {
				semaphoreLock(semaphore34);
				right_wall = check_wall();

				resetGyro(S2);
				repeatUntil(getGyroDegrees(S2) >= 135) {
					setMotorSpeed(motorB, 25);
					setMotorSpeed(motorC, -25);
				}
				resetGyro(S2);

				semaphoreUnlock(semaphore34);
			}

			if(currentDistance >= MAX_DISTANCE && right_wall) {
				semaphoreLock(semaphore34);
				resetGyro(S2);
				repeatUntil(getGyroDegrees(S2) <= -5) {
					setMotorSpeed(motorB, 40);
					setMotorSpeed(motorC, 60);
				}
				resetGyro(S2);
				semaphoreUnlock(semaphore34);
			}
			semaphoreUnlock(semaphore23);
		} else {
			if(!bDoesTaskOwnSemaphore(semaphore34)) {
				semaphoreLock(semaphore34);
			}
		}
	}
}

task go_to_wall() {
	while(true){
		int currentDistance = getUSDistance(S4);

		semaphoreLock(semaphore34);
		if(bDoesTaskOwnSemaphore(semaphore34)) {
			writeDebugStreamLine("GO TO WALL\n");
			if (currentDistance > MAX_DISTANCE) {
				forward(100);
			} else {
				forward(currentDistance);
			}
			semaphoreUnlock(semaphore34);
		}
	}
}

// Main #######################################################################
task main() {
    // Clear debug stream window and wait 1s
    clearDebugStream();
    sleep(500);

	int luz, luzUmbral;
	luz = getColorAmbient(S3);
	sleep(500);
	luzUmbral = luz*1.8;

	// Initialize semaphores
    semaphoreInitialize(semaphore12);
	semaphoreInitialize(semaphore23);
	semaphoreInitialize(semaphore34);

	// Initialize tasks
	startTask(escape);
	startTask(light);
	startTask(follow_wall);
	startTask(go_to_wall);

    // Bucle inicial
    while(true) {
		luz = getColorName(S3);
		if(luz > luzUmbral*1.6) {
			stopAllTasks();
		}
    }
}
