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
    int cnt = 0;

    // Loop
    while(true) {
        // Medir distancia a la pared izquierda
        distance = getUSDistance(S4);

        if(cnt == 0) {
            while(getUSDistance(S4) > MIN_DISTANCE) {
        	    distance = getUSDistance(S4);

                // Led changes
                if(getUSDistance(S4) < 30) {
                    setLEDColor(ledOrange); // If distance < 30 orange led
                } else {
                    setLEDColor(ledGreen); // Else green led
                }

                // Forward
                forward();

                // Print only when distance value change
                if(prev_distance != distance) {
                    writeDebugStreamLine("Distance: %d", distance);
                }

                prev_distance = distance;
            }
        }

        // Comprobamos distancia a izquieda ata que aumenta
        // Paramos
        // Comprobamos distancia a dereita ata que aumenta
        // Paramos
        // Giramos esquerda 90

        resetGyro(S2);
        while (getGyroDegrees(S2) > -90) {
            distance = getUSDistance(S4);
            setMotorSpeed(motorB, -30);
            setMotorSpeed(motorC, 30);
            if (prev_distance < distance) {
                break;
            } else {
                prev_distance = distance;
            }
        }

        // Deshacer recorrido


        /*
        // Si la pared est� demasiado lejos, girar a la izquierda
        if (distance > MIN_DISTANCE) {
            right(TURN_ANGLE);
            wait1Msec(500);
        }
        // Si la pared est� demasiado cerca, girar a la derecha
        else if (distance < MIN_DISTANCE) {
            left(TURN_ANGLE);
            wait1Msec(500);
        }
        // Si la pared est� a la distancia adecuada, avanzar
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
        */
    }
}
///////////////////////////////// End
