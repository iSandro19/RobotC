//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Practice 1
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

// Definitions ################################################################
const int CORNER_ANGLE = 90;
const int ADJUST_ANGLE = 15;
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
    while(getGyroDegrees(S2) > -degrees) {
		setMotorSpeed(motorB, -20);
		setMotorSpeed(motorC, 20);
	}
	stop();
	return;
}

void right(int degrees) {
    while(getGyroDegrees(S2) < degrees) {
		setMotorSpeed(motorB, 20);
		setMotorSpeed(motorC, -20);
	}
	stop();
	return;
}

void perpendicular() {
	writeDebugStreamLine("Perpendicular");
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

bool check_wall() {
	int distance = 0;

	right(CORNER_ANGLE);
	writeDebugStreamLine("Checking wall");
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

// Tasks ######################################################################
/*
task escape() {
	writeDebugStreamLine("Escape task\n");
    // Comprobar si la tarea está activa (no está inhibida por otra).
    // Aviso: proteger acceso concurrente a variable/s.

    // SI no está inhibida:
        // Leer SENSORIZACI�?N necesaria para determinar si la tarea tiene que
        // hacer algo según el estado del entorno.

        // SI tiene que actuar:
            // Inhibir tareas niveles inferiores.
            // Actuar.
        // SINO
            // Desinhibir tareas niveles inferiores.

    // SINO
        // Inhibir tareas inferiores.
}

task light() {
	writeDebugStreamLine("Light task\n");
    // Comprobar si la tarea está activa (no está inhibida por otra).
    // Aviso: proteger acceso concurrente a variable/s.

    // SI no está inhibida:
        // Leer SENSORIZACI�?N necesaria para determinar si la tarea tiene que
        // hacer algo según el estado del entorno.

        // SI tiene que actuar:
            // Inhibir tareas niveles inferiores.
            // Actuar.
        // SINO
            // Desinhibir tareas niveles inferiores.

    // SINO
        // Inhibir tareas inferiores.
}
*/
task follow_wall() {
	writeDebugStreamLine("FOLLOW WALL\n");
	right(CORNER_ANGLE);
	writeDebugStreamLine("    - Checking wall");
	currentDistance = getUSDistance(S4);
	if(currentDistance < MAX_DISTANCE) {
		writeDebugStreamLine("    - Xo ya no sè");
		if(currentDistance <= MIN_DISTANCE) {
			writeDebugStreamLine("Vai pa' tras que chocamos");
			reverse(50);
			sleep(500);
		}
		if(bDoesTaskOwnSemaphore(semaphore)) {
			if(right_wall) {
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
			} else {
				writeDebugStreamLine("    - No se encuentra ninguna pared");
				semaphoreUnlock(semaphore);
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
			writeDebugStreamLine("    - Pared frontal detectada");
			forward(70);
			if(getUSDistance(S4) < (MAX_DISTANCE+MIN_DISTANCE)/2) {
				front_wall = true;
				left(CORNER_ANGLE);
			}
		} else {
			writeDebugStreamLine("    - No se encuentra ninguna pared");
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
		//startTask(escape);
		//startTask(light);
		startTask(follow_wall);
		startTask(go_to_wall);

		abortTimeslice();
    }
}
