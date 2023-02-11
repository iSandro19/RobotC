//#### RobotC File ####//
// Robótica Q8 2022/2023
// Title: Reto 2
// Authors:
//   - Óscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

// Function definition
void forward(speed) {
    setMotorSpeed(motorB, speed);
    setMotorSpeed(motorC, speed);
    return;
}

void reverse(speed) {
    setMotorSpeed(motorB, -speed);
    setMotorSpeed(motorC, -speed);
    return;
}

void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
}

// Main task
task main() {
    // Loop
    while(true) {
        clearDebugStream();

        // While distance > 10, go forward
        while(getUSDistance(S4) > 10) {
            // Forward
            forward(SensorValue[S4]);

            // Print SensorValue only if it change
            if(getUSDistance(S4) != SensorValue[S4]) {
                writeDebugStreamLine("Distance: %d", SensorValue[S4]);
            }
        }

        // Stop and red led
        stop();

        // While distance > 10, go forward
        while(getUSDistance(S4) > 20) {
            // Reverse
            reverse(SensorValue[S4]);

            // Print SensorValue only if it change
            if(getUSDistance(S4) != SensorValue[S4]) {
                writeDebugStreamLine("Distance: %d", SensorValue[S4]);
            }
        }

        stop();
        stopAllTasks();
    }
}
