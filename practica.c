//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Practice 1
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Lei

// Pragma #####################################################################
#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     lightSensor,    sensorEV3_Color, modeEV3Color_Ambient)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

// Definitions ################################################################
const int CORNER_ANGLE = 90;
const int MAX_DISTANCE = 30;
const int MIN_DISTANCE = 15;
const int TURN_DEGREE = 110;

const int LIGHT_FOUND = 35;
const int LIGHT_DETECTED = 15;
const int LIGHT_LOST = 5;

const int DEFEAULT_SPEED = 50;
const int FULL_SPEED = 100;

// Semaphores #################################################################
TSemaphore sem_escape_light;
TSemaphore sem_light_follow;
TSemaphore sem_follow_go;

// Tasks ######################################################################
/*
 * Escape task
 * This task is in charge of escaping from crashes.
 */
task escape() {
    while(true) {
		int currentDistance = getUSDistance(sonarSensor);

		if(getTouchValue(touchSensor) == 1 || currentDistance < MIN_DISTANCE) {
			semaphoreLock(sem_escape_light);
			if(bDoesTaskOwnSemaphore(sem_escape_light)) {
				setMotorSpeed(leftMotor, -DEFEAULT_SPEED);
				setMotorSpeed(rightMotor, -DEFEAULT_SPEED);
				sleep(500);
				resetGyro(gyroSensor);
				repeatUntil(getGyroDegrees(gyroSensor) < -CORNER_ANGLE) {
					setMotorSpeed(leftMotor, -DEFEAULT_SPEED);
					setMotorSpeed(rightMotor, DEFEAULT_SPEED);
				}
				resetGyro(gyroSensor);
				semaphoreUnlock(sem_escape_light);
			}
		}
	}
}

/*
 * Light task
 * This task is in charge of detecting the light.
 */
task light() {
	int luz;

	while(true) {
		luz = getColorAmbient(lightSensor);

		semaphoreLock(sem_escape_light);
		if(bDoesTaskOwnSemaphore(sem_escape_light)) {

			if(bDoesTaskOwnSemaphore(sem_light_follow)) {
				semaphoreUnlock(sem_light_follow);
			}

			if(luz >= LIGHT_DETECTED) {
				semaphoreLock(sem_light_follow);
				setMotorSpeed(leftMotor, DEFEAULT_SPEED);
				setMotorSpeed(rightMotor, -DEFEAULT_SPEED);
				while(luz <= getColorAmbient(lightSensor)) {
					luz = getColorAmbient(lightSensor);
				}
				setMotorSpeed(leftMotor, -DEFEAULT_SPEED);
				setMotorSpeed(rightMotor, DEFEAULT_SPEED);
				while(luz <= getColorAmbient(lightSensor)) {
					luz = getColorAmbient(lightSensor);
				}
				semaphoreUnlock(sem_light_follow);
			}

			if(luz >= LIGHT_DETECTED) {
				stopAllTasks();;
			}

			semaphoreUnlock(sem_escape_light);
		} else {
			if(!bDoesTaskOwnSemaphore(sem_light_follow)) {
				semaphoreLock(sem_light_follow);
			}
		}
	}
}

/*
 * Follow wall task
 * This task is in charge of following the (right) wall.
 */
task follow_wall() {
	bool right_wall = false;

	while(true) {
		// Get current distance
		int currentDistance = getUSDistance(sonarSensor);

		// Lock
		semaphoreLock(sem_light_follow);
		if (bDoesTaskOwnSemaphore(sem_light_follow)) {

			// Unlock
			if(bDoesTaskOwnSemaphore(sem_follow_go)) {
				semaphoreUnlock(sem_follow_go);
			}

			//
			if (currentDistance < MAX_DISTANCE){
				// Lock
				semaphoreLock(sem_follow_go);
					right_wall = true;
					resetGyro(gyroSensor);
					repeatUntil(getGyroDegrees(gyroSensor) <= -TURN_DEGREE) {
						setMotorSpeed(leftMotor, -DEFEAULT_SPEED);
						setMotorSpeed(rightMotor, DEFEAULT_SPEED);
					}
					resetGyro(gyroSensor);

				// Unlock
				semaphoreUnlock(sem_follow_go);
			}

			if(currentDistance > MAX_DISTANCE && right_wall) {
				semaphoreLock(sem_follow_go);
					resetGyro(gyroSensor);
					repeatUntil(getGyroDegrees(gyroSensor) >= 5) {
						setMotorSpeed(leftMotor, DEFEAULT_SPEED + 10);
						setMotorSpeed(rightMotor, DEFEAULT_SPEED - 10);
					}
					resetGyro(gyroSensor);

				// Unlock
				semaphoreUnlock(sem_follow_go);
			}

			// Unlock
			semaphoreUnlock(sem_light_follow);
		} else {
			// Lock
			if(!bDoesTaskOwnSemaphore(sem_follow_go)) {
				semaphoreLock(sem_follow_go);
			}
		}
	}
}

/*
 * Go to wall task
 * This task is in charge of going to the (front) wall.
 */
task go_to_wall() {
	while(true){
		// Get current distance
		int currentDistance = getUSDistance(sonarSensor);

		// Lock
		semaphoreLock(sem_follow_go);
		if(bDoesTaskOwnSemaphore(sem_follow_go)) {
			// 100 speed
			if (currentDistance > MAX_DISTANCE) {
				setMotorSpeed(leftMotor, FULL_SPEED);
				setMotorSpeed(rightMotor, FULL_SPEED);
			
			// Linear distance speed
			} else {
				setMotorSpeed(leftMotor, currentDistance + 20);
				setMotorSpeed(rightMotor, currentDistance + 20);
			}

			// Unlock
			semaphoreUnlock(sem_follow_go);
		}
	}
}

// Main #######################################################################
task main() {
	// Clear debug stream window and short sleep
    clearDebugStream();
    sleep(500);

	// Set arm motor up
	setMotorSpeed(armMotor, FULL_SPEED);
	sleep(100);

	// Initialize semaphores
    semaphoreInitialize(sem_escape_light);
	semaphoreInitialize(sem_light_follow);
	semaphoreInitialize(sem_follow_go);

	// Start tasks
	startTask(escape);
	startTask(light);
	startTask(follow_wall);
	startTask(go_to_wall);
}
