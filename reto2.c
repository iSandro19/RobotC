//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Reto 2
// Authors:
//   - O?scar Alejandro Manteiga Seoane
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

    // Loop
    while(true) {
        clearDebugStream();

        // While distance > 10, go forward
        while(getUSDistance(S4) > 10) {
        		int distance = getUSDistance(S4);

            // Forward
            forward(distance);

            // Print only when distance value change
            if(prev_distance != distance) {
            	  writeDebugStreamLine("Forward distance: %d", distance);
            }

            // Update previous value
            prev_distance = distance;
        }

        // Stop
        stop();

        // Reset prev_distance
        prev_distance = 0;

        // Print stop distance value
        writeDebugStreamLine("Stop at: %d", SensorValue[S4]);
        sleep(1000);

        // While distance > 10, go forward
        while(getUSDistance(S4) < 40) {
        		distance = getUSDistance(S4);

            // Reverse
            reverse(distance);

             // Print only when distance value change
            if(prev_distance != distance) {
                writeDebugStreamLine("Reverse distance: %d", distance);
            }

            // Update previous value
            prev_distance = distance;
        }

        // Shut down
        stop();
        stopAllTasks();
    }
}
///////////////////////////////// End
