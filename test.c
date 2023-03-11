// Definitions ################################################################
const int CORNER_ANGLE = 90;
const int MAX_DISTANCE = 30;
const int MIN_DISTANCE = 15;

bool right_wall = false;
bool front_wall = false;
int currentDistance = 0;

// Semaphores #################################################################
TSemaphore semaphore;

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
	writeDebugStream("Left %d\n", getGyroDegrees(S2));
    while(getGyroDegrees(S2) > -degrees) {
        setMotorSpeed(motorB, -20);
        setMotorSpeed(motorC, 20);
    }
    stop();
    return;
}

void right(int degrees) {
	writeDebugStream("Right %d\n", getGyroDegrees(S2));
    while(getGyroDegrees(S2) < degrees) {
        setMotorSpeed(motorB, 20);
        setMotorSpeed(motorC, -20);
    }
    stop();
    return;
}

// Tasks ######################################################################
task follow_wall() {
    writeDebugStreamLine("FOLLOW WALL\n");
    right(CORNER_ANGLE);
    writeDebugStreamLine("    - Checking wall");
    currentDistance = getUSDistance(S4);
    if(currentDistance < MAX_DISTANCE) {
        writeDebugStreamLine("    - Xo ya no se");
        if(currentDistance <= MIN_DISTANCE) {
            writeDebugStreamLine("Vai pa' tras que chocamos");
            reverse(50);
            sleep(500);
        }

        if(bDoesTaskOwnSemaphore(semaphore)) {
		    writeDebugStreamLine("    - Pared derecha detectada");
		    left(CORNER_ANGLE-4);
		    clearTimer(T1);
		    while(time1[T1] < 1000) {
		        forward(70);
		        if(getUSDistance(S4) <= MIN_DISTANCE) {
		            left(CORNER_ANGLE);
		            break;
		        }
            }
        }
    } else {
        writeDebugStreamLine("    - Ta mu lejos");
        if(!bDoesTaskOwnSemaphore(semaphore)) {
            semaphoreLock(semaphore);
        }
    }
}

task go_to_wall() {
    writeDebugStreamLine("GO TO WALL\n");
    currentDistance = getUSDistance(S4);
    if(currentDistance > MAX_DISTANCE) {
        writeDebugStreamLine("    - Ta mu lejos");
        if(!bDoesTaskOwnSemaphore(semaphore)) {
            semaphoreLock(semaphore);
        }
    }

    if(bDoesTaskOwnSemaphore(semaphore)) {
        writeDebugStreamLine("    - Checking wall");
        if(currentDistance <= (MAX_DISTANCE+MIN_DISTANCE)/2) {
            front_wall = true;
        } else {
            front_wall = false;
        }

        if(front_wall == false) {
            writeDebugStreamLine("    - Pared frontal no detectada");
            forward(70);
            while(getUSDistance(S4) > MIN_DISTANCE) {
                wait1Msec(50);
            }
            left(CORNER_ANGLE);
            semaphoreUnlock(semaphore);
        } else {
            writeDebugStreamLine("    - Pared frontal ya detectada");
            semaphoreUnlock(semaphore);
        }
    }
}

// Main #######################################################################
task main() {
	// Clear debug stream window
	clearDebugStream();
	sleep(1000);
	semaphoreInitialize(semaphore);
	resetGyro(S2);

	// Bucle inicial
	while(true) {
		writeDebugStreamLine("START");
		startTask(follow_wall);
		startTask(go_to_wall);

		sleep(500);
	}
}
