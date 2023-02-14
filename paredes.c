//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Wall following
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

///////////////////////////////// Function definition
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
			  setMotorSpeed(motorB, -30);
		    setMotorSpeed(motorC, 30);
		    writeDebugStreamLine("%d", getGyroDegrees(S2));
		}
		return;
}

// Turn right
void right(int degrees) {
	  resetGyro(S2);
    while (getGyroDegrees(S2) < degrees) {
			  setMotorSpeed(motorB, 30);
		    setMotorSpeed(motorC, -30);
		}
		return;
}

// Stop
void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
}
/////////////////////////////////

///////////////////////////////// Constant definition
const int TURN_ANGLE = 30;
const int CORNER_ANGLE = 90;
const int MIN_DISTANCE = 20;
/////////////////////////////////

///////////////////////////////// Main task
task main() {
    // Variables
    int prev_distance = 0;
    int distance = 0;

    // Loop
    while(true) {
        // Medir distancia a la pared izquierda
    		left(CORNER_ANGLE);
        distance = getUSDistance(S4);

        // Si no hay pared a la izquierda, girar a la izquierda
        if (distance == 255) {
            reverse(50);
            wait1Msec(500);
            setMotorSpeed(motorB, 0);
            wait1Msec(1000);
            left(TURN_ANGLE);
            wait1Msec(500);
        }
        // Si la pared está demasiado lejos, girar a la izquierda
        else if (distance > MIN_DISTANCE) {
            left(TURN_ANGLE);
            wait1Msec(500);
        }
        // Si la pared está demasiado cerca, girar a la derecha
        else if (distance < MIN_DISTANCE) {
            right(TURN_ANGLE);
            wait1Msec(500);
        }
        // Si la pared está a la distancia adecuada, avanzar
        else {
            forward(50);
        }

        // Detectar esquinas
        int front_distance = getUSDistance(S4);
        if (front_distance < 30) {
            right(CORNER_ANGLE);
            wait1Msec(500);
        }

        // Actualizar distancia anterior
        prev_distance = distance;
    }

    // Shut down
    stop();
    stopAllTasks();
}
///////////////////////////////// End
