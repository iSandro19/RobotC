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

// Check wall
void check_wall() {
		int prev_distance = 0;
    int distance = 0;

		resetGyro(S2);
		clearTimer(T1);
		while (getGyroDegrees(S2) > -90) {
		    distance = getUSDistance(S4);
		    setMotorSpeed(motorB, -30);
				setMotorSpeed(motorC, 30);
		    if (prev_distance < distance) {
		    		setMotorSpeed(motorB, 30);
						setMotorSpeed(motorC, -30);
						sleep(time1[T1]);
		        setMotorSpeed(motorB, 0);
    				setMotorSpeed(motorC, 0);
		        return;
		    } else {
		        prev_distance = distance;
		    }
		}

		prev_distance = 0;

		resetGyro(S2);
		clearTimer(T1);
		while (getGyroDegrees(S2) < 90) {
		    distance = getUSDistance(S4);
		    setMotorSpeed(motorB, 30);
				setMotorSpeed(motorC, -30);
		    if (prev_distance < distance) {
		    		setMotorSpeed(motorB, -30);
						setMotorSpeed(motorC, 30);
						sleep(time1[T1]);
		        setMotorSpeed(motorB, 0);
    				setMotorSpeed(motorC, 0);
		        return;
		    } else {
		        prev_distance = distance;
		    }
		}
}

// Stop
void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
}
/////////////////////////////////

///////////////////////////////// Variable definitions
const int TURN_ANGLE = 25;
const int CORNER_ANGLE = 85;
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
                forward(100);

                // Print only when distance value change
                if(prev_distance != distance) {
                    writeDebugStreamLine("Distance: %d", distance);
                }

                prev_distance = distance;
            }

            cnt = cnt + 1;
        }

        check_wall();

        // Giramos esquerda 90
        left(CORNER_ANGLE);

        // Avanzamos
        if (getUSDistance(S4) > MIN_DISTANCE) {
        		forward(50);
        		sleep(2000);
      	} else {
      			stopAllTasks();
      	}

        stop();
       	sleep(1000);

        // Comprobamos a dereita
        right(CORNER_ANGLE);
    }
}
///////////////////////////////// End
