//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Wall following
// Authors:
//   - Oscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

///////////////////////////////// Definitions
const int TURN_ANGLE = 30;
const int CORNER_ANGLE = 90;
const int MIN_DISTANCE = 20;

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
		    //writeDebugStreamLine("%d", getGyroDegrees(S2));
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

    distance = getUSDistance(S4);
    prev_distance = distance;

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
    				writeDebugStreamLine("Left check");
		        break;
		    } else {
		        prev_distance = distance;
		    }
		}

		distance = getUSDistance(S4);
    prev_distance = distance;

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
    				writeDebugStreamLine("Right check");
		        break;
		    } else {
		        prev_distance = distance;
		    }
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

///////////////////////////////// Main task
task main() {
    // Variables
    int prev_distance = 0;
    int distance = 0;

    // Medir distancia a la pared izquierda
    distance = getUSDistance(S4);

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
				/*
        // Print only when distance value change
        if(prev_distance != distance) {
            writeDebugStreamLine("Distance: %d", distance);
      	}
      	*/
        prev_distance = distance;
    }

    // Loop
    while(true) {
        check_wall();
        left(CORNER_ANGLE);

        clearTimer(T1);
        while (getUSDistance(S4) > MIN_DISTANCE) {
        		forward(50);
        		if(time1[T1] >= 2000) {
        				stop();
        				right(CORNER_ANGLE);
        				check_wall();
        				left(CORNER_ANGLE);
        				clearTimer(T1);
        		}
      	}
    }
}
///////////////////////////////// End
