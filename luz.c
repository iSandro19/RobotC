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


//Follow brightest light direction
void lightFollow(){
//Boleans for storing previous rotation direction
bool wasRotatingR, wasRotatingL;
//Displays brightest direction
displayStringAt(90, 120, "Light Dir = %d",BrightLight);//Display
//Display sound level label
displayStringAt(5, 120, "Sound Level");//Display

		//rotate Left
	if(BrightLight < 4 && BrightLight > 0){
			motor[motorB] = 10;
			motor[left] = -10;
			//Find direction of rotation
			wasRotatingR = false;
			wasRotatingL = true;
		}
		//Rotate Right
		else if(BrightLight > 6){

				motor[right] = -10;
				motor[left] = 10;
				//Find direction of rotation
				wasRotatingR = true;
				wasRotatingL = false;
		}
		//Drive forward while slightly turning right
		else if (BrightLight == 4){
			//If robot was rotating than stop, so robot doesn't pass light direction
			if(wasRotatingR || wasRotatingL){
			//stop motors
				motor[right] = 0;
				motor[left] = 0;
				wait1Msec(500);
		}
			//set right slightly faster than left for slow tilt
			motor[right] = 30;
			motor[left] = 28;
			wasRotatingR = false;
			wasRotatingL = false;
		}
		//Drive forward
		else if (BrightLight == 5){
			//If robot was rotating than stop, so robot doesn't pass light direction
			if(wasRotatingR || wasRotatingL){
				//stop motors
				motor[right] = 0;
				motor[left] = 0;
				wait1Msec(500);
		//play sound when directly following light
			setSoundVolume(20);
		//playSound
			playSound(soundFastUpwardTones);
		}
			motor[right] = 30;
			motor[left] = 30;
			//Set both rotation values to false: robot is driving forward
			wasRotatingR = false;
			wasRotatingL = false;
		}
		//Drive forward while slightly turning left
		else if (BrightLight == 6 ){
			//If robot was rotating than stop, so robot doesn't pass light direction
			if(wasRotatingR || wasRotatingL){
				motor[right] = 0;
				motor[left] = 0;
				wait1Msec(500);
		}
			motor[right] = 28;
			motor[left] = 30;
			//Set both rotation values to false: robot is driving forward
			wasRotatingR = false;
			wasRotatingL = false;
		}
		//When no brightest light direction is detected, continue rotating in last
		//rotation direction, or rotate left if robot wasn't previously rotating
		else{
			if(wasRotatingR){
				motor[right] = -10;
				motor[left] = 10;
			}
			else if(wasRotatingL){
				motor[right] = 10;
				motor[left] = -10;
			}
			else{
				motor[right] = 10;
				motor[left] = -10;
				wasRotatingL = true;
			}
		}//end else

}//end light follow

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
