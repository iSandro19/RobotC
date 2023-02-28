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

// Semaphores #################################################################
TSemaphore escape_light;
TSemaphore light_follow;
TSemaphore follow_go;

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
	resetGyro(S2);
    while (getGyroDegrees(S2) > -degrees) {
		setMotorSpeed(motorB, -20);
		setMotorSpeed(motorC, 20);
	}
	stop();
	return;
}

void right(int degrees) {
	resetGyro(S2);
    while (getGyroDegrees(S2) < degrees) {
		setMotorSpeed(motorB, 20);
		setMotorSpeed(motorC, -20);
	}
	stop();
	return;
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

	// If one wall is too far
    /*
	if(front_distance > MAX_DISTANCE ||
	   left_distante > MAX_DISTANCE ||
	   right_distante > MAX_DISTANCE) {
		writeDebugStreamLine("Oh, something wrong with the walls");
		stopAllTasks();
	}
    */

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
	/*
	} else if (right_distante > front_distance && right_distante > left_distante) {
		writeDebugStreamLine("Adjust a little bit to the left");
		left(ADJUST_ANGLE);
	} else if (left_distante > front_distance && left_distante > right_distante) {
		writeDebugStreamLine("Adjust a little bit to the right");
		left(ADJUST_ANGLE);
	*/
	}
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

// Tasks ######################################################################
task escape() {
    // Comprobar si la tarea está activa (no está inhibida por otra).
    // Aviso: proteger acceso concurrente a variable/s.
    
    // SI no está inhibida:
        // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
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
    // Comprobar si la tarea está activa (no está inhibida por otra).
    // Aviso: proteger acceso concurrente a variable/s.
    
    // SI no está inhibida:
        // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
        // hacer algo según el estado del entorno.
        
        // SI tiene que actuar:
            // Inhibir tareas niveles inferiores.
            // Actuar.
        // SINO
            // Desinhibir tareas niveles inferiores.
    
    // SINO
        // Inhibir tareas inferiores.
}

task follow_wall() {
    while(true) {
        // Comprobar si la tarea está activa (no está inhibida por otra).
        // Aviso: proteger acceso concurrente a variable/s.

        
        // SI no está inhibida:
            // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
            // hacer algo según el estado del entorno.
            
            // SI tiene que actuar:
                // Inhibir tareas niveles inferiores.
                // Actuar.
            // SINO
                // Desinhibir tareas niveles inferiores.
        
        // SINO
            // Inhibir tareas inferiores.
    }
}

task go_to_wall() {
    // Comprobar si la tarea está activa (no está inhibida por otra).
    // Aviso: proteger acceso concurrente a variable/s.
    // SI no está inhibida:
    if(bDoesTaskOwnSemaphore(follow_go)) {
        // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
        // hacer algo según el estado del entorno.
        
        // SI tiene que actuar:
            // Inhibir tareas niveles inferiores.
            // Actuar.
        // SINO
            // Desinhibir tareas niveles inferiores.
    // SINO
    } else {
        // Inhibir tareas inferiores.
    }
}

// Main #######################################################################
task main() {
    // Clear debug stream window
    clearDebugStream();
    
    // Inicializar variables.
	bool right_wall = false;
	bool front_wall = false;

    // Sleep de seguridad
    sleep(2000);

    // Inicializar semáforos
    //semaphoreInitialize(escape_light);
    //semaphoreInitialize(light_follow);
    semaphoreInitialize(follow_go);

    // Inicializar tareas
    //startTask(escape);
    //startTask(light);
    startTask(follow_wall);
    startTask(go_to_wall);

    // Bucle inicial
    while(true) {
        abortTimeslice();
    }
}