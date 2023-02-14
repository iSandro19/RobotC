//#### RobotC File ####//
// Robotica Q8 2022/2023
// Title: Light following
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

// Stop
void stop() {
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    return;
}
/////////////////////////////////

///////////////////////////////// Main task
task main() {
    while(true) {
    	  // Solo implementable en el robot real

        // Shut down
		    stop();
		    stopAllTasks();
    }
}
///////////////////////////////// End
