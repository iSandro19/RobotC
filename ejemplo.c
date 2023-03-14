#pragma config(Sensor, S1,     Touch,          sensorEV3_Touch)
#pragma config(Sensor, S2,     gyro,           sensorEV3_Gyro)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     Sonar,          sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          claw,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorEV3_Large, PIDControl, encoder)

//Distancias
const int distToEscape = 18;
const int distToApproach = 30;
int currentDistance = 0;
bool isTurning = false;

//Semaforos
TSemaphore semaphore;
int currentPriority = 0;

//Contadores
int approachCount = 0;
int escapeCount = 0;

//Luz
int initialColorAmbient = 0;
int currentColor = 0;

void turnLeft(){
//Resets the gyro on port 2 to 0 degrees
	if (isTurning == false){
		isTurning = true;
		resetGyro(gyro);
	}
 //Keep looping until the gyro sensor reads greater
 //than 90 degrees

 repeatUntil(getGyroDegrees(gyro) > 90)
{
   //writeDebugStream("giro: %d\n", getGyroDegrees(gyro));
  //Point turn to the left
  setMotorSpeed(motorC, -30);
  setMotorSpeed(motorB, 30);
 }
 isTurning = false;

 //Stop the motors at the end of the turn
 setMotorSpeed(motorB, 0);
 setMotorSpeed(motorC, 0);
}

void stay_put(){
	motor[left] = 0;
	motor[right] = 0;
}

void close_claw(){
	motor[claw] = 30;
	sleep(300);
	motor[claw] = 0;
}

void turnRight(){
//Resets the gyro on port 2 to 0 degrees
	if (isTurning == false){
		isTurning = true;
		resetGyro(gyro);
	}
 //Keep looping until the gyro sensor reads greater
 //than 90 degrees

 repeatUntil(getGyroDegrees(gyro) < -90)
 {
   //writeDebugStream("giro: %d\n", getGyroDegrees(gyro));
  //Point turn to the left
  setMotorSpeed(motorC, 30);
  setMotorSpeed(motorB, -30);
 }

 isTurning = false;

 //Stop the motors at the end of the turn
 setMotorSpeed(motorB, 0);
 setMotorSpeed(motorC, 0);
}

void go_backwards(){
	motor[left] = -30;
	motor[right] = -30;
	//sleep(1000);
}

void go_forward(){
	motor[left] = 30;
	motor[right] = 30;
}

void unlock(int priority){
	currentPriority = priority;
	semaphoreUnlock(semaphore);
}

void lock(int priority){
	if (priority >= currentPriority){
		if (bDoesTaskOwnSemaphore(semaphore) == true) {
			unlock(priority);
		}
		semaphoreLock(semaphore);
	}
}

task escape() {
	writeDebugStream("Escape\n");
	currentDistance = SensorValue[Sonar];

	if ((currentDistance < distToEscape) || SensorValue[Touch]) {
		if (!bDoesTaskOwnSemaphore(semaphore)) {
			lock(4);
		}
	}

	if (bDoesTaskOwnSemaphore(semaphore) == true) {
		setLEDColor(ledRed);


		//go_backwards();
		//sleep(1000);
		turnRight();

		unlock(0);
	} else {
		escapeCount = 0;
	}
}

task chase_light() {
	writeDebugStream("Chase_Light\n");
	currentColor = getColorAmbient(colorSensor);
	//writeDebugStream("currentColor: %d initialColorAmbient %d\n", currentColor, initialColorAmbient);
	//int currentColor = 0;
	//getColorRGB(colorSensor, redValue, greenValue, blueValue);
	//currentColor = redValue;

	if (currentColor > initialColorAmbient + 20) {
		if (!bDoesTaskOwnSemaphore(semaphore)) {
			lock(5);
		}
	}

	//-TODO remove this red only check
	if (bDoesTaskOwnSemaphore(semaphore)) {
		setLEDColor(ledGreen);
		go_forward();
		unlock(0);
	}

}

task follow_walls() {
	writeDebugStream("Follow_Walls\n");
	int currentDist = SensorValue[Sonar];
	int error = 0;
	int derivative = 0;
	float correction = 0;
	int baseSpeed = 30;
	//teorico
	//int targetDist = 50;
	//real
	int targetDist = 80;

	//Teorico
	/*float kp = 1.5;
	float ki = 0;
	float kd = 0.01;*/

	//Real
	float kp = 1.2;
	float ki = 0;
	float kd = 0.1;

	int lastError = 0;
	int integral = 0;

	if (currentDistance < distToApproach && currentDistance > distToEscape) {
		if (!bDoesTaskOwnSemaphore(semaphore)) {
			lock(2);
		}
	}

	if (bDoesTaskOwnSemaphore(semaphore)) {
		setLEDColor(ledGreen);
		// Read the sensor
		currentDist = SensorValue[Sonar];
		error = targetDist - currentDist;
		integral = error + integral;
		derivative = error + lastError;
		correction = (kp*error + ki*integral + kd*derivative);
		motor[right] = baseSpeed - correction;
		motor[left] = baseSpeed + correction;
		lastError = error;

		unlock(0);
	}

}

task approach_walls() {
	writeDebugStream("Approach_Walls\n");
	currentDistance = SensorValue[Sonar];
	//writeDebugStream("distancia: %d\n", currentDistance);
	if (currentDistance > distToApproach) {
		if (!bDoesTaskOwnSemaphore(semaphore)) {
			lock(1);
		}
	}

	if (bDoesTaskOwnSemaphore(semaphore)) {
		setLEDColor(ledOrange);
			//writeDebugStream("approachCount: %d\n", approachCount);
		if (approachCount < 1250 && approachCount >= 1000){
			turnLeft();
		} else if (approachCount >= 1250){
			approachCount = 0;
		} else {
			go_forward();
		}

		approachCount++;
		unlock(0);
	} else {
		approachCount = 0;
	}
}

task main()
{
	close_claw();
	semaphoreInitialize(semaphore);

	initialColorAmbient = getColorAmbient(colorSensor);
	sleep(1000);
	initialColorAmbient = getColorAmbient(colorSensor);
	writeDebugStream("initialColorAmbient: %d\n", initialColorAmbient);

	resetGyro(gyro);

	while(true){
		startTask(escape);
		//startTask(chase_light);
		startTask(follow_walls);
		startTask(approach_walls);
		//wait1Msec(250);
		abortTimeslice();
	}


}