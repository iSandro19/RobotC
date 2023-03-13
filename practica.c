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

// Semaphores #################################################################
TSemaphore semaphore12;
TSemaphore semaphore23;
TSemaphore semaphore34;

// Tasks ######################################################################
task escape() {
    while(true) {
		int currentDistance = getUSDistance(S4);

		if(getTouchValue(S1) == 1 || currentDistance < MIN_DISTANCE) {
			semaphoreLock(semaphore12);
			if(bDoesTaskOwnSemaphore(semaphore12)) {
				writeDebugStreamLine("ESCAPE\n");
				setMotorSpeed(motorB, -50);
				setMotorSpeed(motorC, -50);
				sleep(500);
				resetGyro(S2);
				repeatUntil(getGyroDegrees(S2) < -CORNER_ANGLE) {
					setMotorSpeed(motorB, -30);
					setMotorSpeed(motorC, 30);
				}
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

			if(bDoesTaskOwnSemaphore(semaphore23)) {
				semaphoreUnlock(semaphore23);
			}

			if(luz > luzUmbral) {
				semaphoreLock(semaphore23);
				writeDebugStreamLine("LIGHT 2\n");
				setMotorSpeed(motorB, 30);
				setMotorSpeed(motorC, -30);
				while(luz <= getColorAmbient(S3)) {
					luz = getColorAmbient(S3);
				}
				writeDebugStreamLine("LIGHT 3\n");
				setMotorSpeed(motorB, -30);
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
	bool pared = false;
	
	while(true) {
		int currentDistance = getUSDistance(S4);

		semaphoreLock(semaphore23);
		if (bDoesTaskOwnSemaphore(semaphore23)) { //Si no estamos sigiuiendo luz
            writeDebugStreamLine("Seguir pared");

			if(bDoesTaskOwnSemaphore(semaphore34)) {
				semaphoreUnlock(semaphore34);
			} //Si siguiendo pared: dejar seguir

			if (currentDistance < 25){
				semaphoreLock(semaphore34); //Seguir pared
					pared = true;
					resetGyro(S2);
					repeatUntil(getGyroDegrees(S2) <= -110) {
						setMotorSpeed(motorB, -25);
						setMotorSpeed(motorC, 25);
					}
					resetGyro(S2);
				semaphoreUnlock(semaphore34); //Dejar seguir pared
			}

			if(currentDistance > 25 && pared) {
				semaphoreLock(semaphore34); //Seguir pared
					resetGyro(S2);
					repeatUntil(getGyroDegrees(S2) >= 5) {
						setMotorSpeed(motorB, 60);
						setMotorSpeed(motorC, 30);
					}
					resetGyro(S2);
				semaphoreUnlock(semaphore34); //Dejar seguir pared
			}
			semaphoreUnlock(semaphore23); //Dejar libre seguir luz
		} else {
			if(!bDoesTaskOwnSemaphore(semaphore34)) {
				semaphoreLock(semaphore34);
			} //Si no siguiendo pared: seguir pared
		}
	}
}

task go_to_wall() {
	while(true){
		int currentDistance = getUSDistance(S4);

		semaphoreLock(semaphore34);
		if(bDoesTaskOwnSemaphore(semaphore34)) {
			if (currentDistance > MAX_DISTANCE) {
				writeDebugStreamLine("GO TO WALL - 100\n");
				setMotorSpeed(motorB, 100);
				setMotorSpeed(motorC, 100);
			} else {
				writeDebugStreamLine("GO TO WALL - Distance\n");
				setMotorSpeed(motorB, currentDistance);
				setMotorSpeed(motorC, currentDistance);
			}
			semaphoreUnlock(semaphore34);
		}
	}
}

// Main #######################################################################
task main() {
    // Clear debug stream window and wait 1s
    clearDebugStream();
	setMotorSpeed(motorA, 100);
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
