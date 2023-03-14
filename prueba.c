// RobotC file

task main()
{
    // Forward
    setMotorSpeed(motorB, 100);
    setMotorSpeed(motorC, 100);
    sleep(1000);

    // Right
    setMotorSpeed(motorB, 50);
    setMotorSpeed(motorC, -50);
    sleep(275);

    // Forward
    setMotorSpeed(motorB, 100);
    setMotorSpeed(motorC, 100);
    sleep(1500);

    // Left
    setMotorSpeed(motorB, -50);
    setMotorSpeed(motorC, 50);
    sleep(275);

    // Forward
    setMotorSpeed(motorB, 100);
    setMotorSpeed(motorC, 100);
    sleep(2000);

    // Right
    setMotorSpeed(motorB, 50);
    setMotorSpeed(motorC, -50);
    sleep(375);

    // Forward
    setMotorSpeed(motorB, 100);
    setMotorSpeed(motorC, 100);
    sleep(2800);

    // Left
    setMotorSpeed(motorB, -50);
    setMotorSpeed(motorC, 50);
    sleep(275);

    // Forward
    setMotorSpeed(motorB, 100);
    setMotorSpeed(motorC, 100);
    sleep(2000);

    // Celebrate
    setMotorSpeed(motorB, 100);
    setMotorSpeed(motorC, -100);
    sleep(10000);
    setMotorSpeed(motorB, -100);
    setMotorSpeed(motorC, 100);
    sleep(10000);

    // Stop
    setMotorSpeed(motorB, 0);
    setMotorSpeed(motorC, 0);
    sleep(1);
}
