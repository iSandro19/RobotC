//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Practice 1
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Lei

// Definitions ################################################################
const int CORNER_ANGLE = 90;
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
				setMotorSpeed(motorB, 30);
				setMotorSpeed(motorC, -30);
				while(luz <= getColorAmbient(S3)) {
					luz = getColorAmbient(S3);
				}
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
		if (bDoesTaskOwnSemaphore(semaphore23)) {

			if(bDoesTaskOwnSemaphore(semaphore34)) {
				semaphoreUnlock(semaphore34);
			}

			if (currentDistance < 25){
				semaphoreLock(semaphore34);
					pared = true;
					resetGyro(S2);
					repeatUntil(getGyroDegrees(S2) <= -110) {
						setMotorSpeed(motorB, -25);
						setMotorSpeed(motorC, 25);
					}
					resetGyro(S2);
				semaphoreUnlock(semaphore34);
			}

			if(currentDistance > 25 && pared) {
				semaphoreLock(semaphore34);
					resetGyro(S2);
					repeatUntil(getGyroDegrees(S2) >= 5) {
						setMotorSpeed(motorB, 60);
						setMotorSpeed(motorC, 30);
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
			if (currentDistance > MAX_DISTANCE) {
				setMotorSpeed(motorB, 100);
				setMotorSpeed(motorC, 100);
			} else {
				setMotorSpeed(motorB, currentDistance);
				setMotorSpeed(motorC, currentDistance);
			}
			semaphoreUnlock(semaphore34);
		}
	}
}

// Main #######################################################################
task main() {
    clearDebugStream();
	setMotorSpeed(motorA, 100);
    sleep(500);

	int luz, luzUmbral;
	luz = getColorAmbient(S3);
	sleep(500);
	luzUmbral = luz*1.8;
    semaphoreInitialize(semaphore12);
	semaphoreInitialize(semaphore23);
	semaphoreInitialize(semaphore34);

	startTask(escape);
	startTask(light);
	startTask(follow_wall);
	startTask(go_to_wall);

    while(true) {
		luz = getColorName(S3);
		if(luz > luzUmbral*1.6) {
			stopAllTasks();
		}
    }
}
