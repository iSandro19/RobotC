//#### RobotC File ####//
// Robótica Q8 2022/2023
// Title: Reto 1
// Authors:
//   - Óscar Alejandro Manteiga Seoane
//   - Antonio Vila Leis

// Function definition
void forward() {
    setMotorSpeed(motorB, 50);
    setMotorSpeed(motorC, 50);
    return;
}

void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
}

void right() {
    setMotorSpeed(motorB, 20);
    setMotorSpeed(motorC, -20);
    return;
}

// Main task
task main() {
    // Loop
    while(true) {
        clearDebugStream();

        // If distance > 15, go forward
        while(getUSDistance(S4) > 15) {
            // If distance < 30 Oorange led
            if(getUSDistance(S4) < 30) {
                setLEDColor(ledOrange);
            // Else green led
            } else {
                setLEDColor(ledGreen);
            }

            // Print in DebugStream only if previous value is different
            if(getUSDistance(S4) != SensorValue[S4]) {
                writeDebugStreamLine("Distance: %d", SensorValue[S4]);
            }

            // Forward
            forward();
        }

        // Stop and red led
        setLEDColor(ledRed);
        stop();

        // Reset gyro and timer
        resetGyro(S2);
        clearTimer(T1);

        // Turn right 90º using the gyroscope
        while(getGyroDegrees(S2) < 90) {
            right();

            if(getGyroDegrees(S2) != SensorValue[S2]) {
                writeDebugStreamLine("Gyro: %d", SensorValue[S2]);
            }

            // If right turn last more than 2s, stop and red flash led
            if(time1[T1] > 2000) {
                stop();
                setLEDColor(ledRedFlash);
                writeDebugStreamLine("ERROR: se ha detectado que el robot no puede moverse");
                stopAllTasks();
            }
        }
    }
}
