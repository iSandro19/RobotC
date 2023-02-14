//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Reto 1
// Authors:
//   - O?scar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

///////////////////////////////// Function definition
// Forward
void forward() {
    setMotorSpeed(motorB, 50);
    setMotorSpeed(motorC, 50);
    return;
}

// Stop
void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
}

// Right turn
void right() {
    setMotorSpeed(motorB, 20);
    setMotorSpeed(motorC, -20);
    return;
}

///////////////////////////////// Main task
task main() {
	  // Variables
	  int prev_distance = 0;
	  int distance = 0;
	  int prev_gyro = 0;
	  int gyro = 0;

    // Loop
    while(true) {
        clearDebugStream();

        // If distance > 15, go forward
        while(getUSDistance(S4) > 15) {
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

        // Stop and red led
        setLEDColor(ledRed);
        stop();

        // Reset gyro and timer
        resetGyro(S2);
        clearTimer(T1);

        // Turn right 90 degrees using the gyroscope
        while(getGyroDegrees(S2) < 90) {
        	  gyro = getGyroDegrees(S2);
            right();

            if(prev_gyro != gyro) {
                writeDebugStreamLine("Gyro: %d", gyro);
            }

            // If right turn last more than 2s, stop and red flash led
            if(time1[T1] > 2000) {
                stop();
                setLEDColor(ledRedFlash);
                writeDebugStreamLine("ERROR: detectado robot atascado");
                stopAllTasks();
            }

            prev_gyro = gyro;
        }
    }
}
///////////////////////////////// End
