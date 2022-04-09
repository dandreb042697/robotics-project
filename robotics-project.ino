#include <Servo.h>
#include <AFMotor.h>

const int PIN_SERVO = 10;
const int PIN_TRIG = A0;
const int PIN_ECHO = A2;
const int PIN_BUZZER = 5;

#define MAX_DISTANCE 200
#define MAX_SPEED 150 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

AF_DCMotor motor2(2, MOTOR1_A);
// AF_DCMotor motor3(3, MOTOR34_1KHZ);

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

Servo servo;

long duration;
int ultraSonicDistance; // centimeter

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    servo.attach(PIN_SERVO);

    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    ultraSonicDistance = readDistance();
    delay(100);
    ultraSonicDistance = readDistance();
    delay(100);
    ultraSonicDistance = readDistance();
    delay(100);
}

void loop()
{

    int distanceLeft = 0;
    int distanceRight = 0;
    delay(40);

    ultraSonicDistance = lookForward();

    if (ultraSonicDistance <= 15)
    {

        moveStop();
        delay(100);
        moveBackward();
        delay(300);
        moveStop();
        delay(200);

        playBuzzer();

        // Get distance from left and right
        distanceRight = lookRight();
        delay(300);
        distanceLeft = lookLeft();
        delay(300);

        if (distanceRight >= distanceLeft)
        {
            turnRight();
            moveStop();
        }
        else
        {
            turnLeft();
            moveStop();
        }
    }
    else
    {
        moveForward();
    }
}

void playBuzzer()
{
    tone(PIN_BUZZER, 1500, 300);
    delay(500);
    // tone(PIN_BUZZER, 500, 300);
    // delay(500);
    // tone(PIN_BUZZER, 1500, 300);
    // delay(300);
}

int readDistance()
{
    // Clears the trigPin condition
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    int duration = pulseIn(PIN_ECHO, HIGH);

    return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

int lookForward()
{
    servo.write(90);
    int value = readDistance();
    readDistance();

    return value;
}

int lookLeft()
{
    servo.write(180);
    int value = readDistance();
    readDistance();
    servo.write(90);

    return value;
}

int lookRight()
{
    servo.write(0);

    int value = readDistance();
    readDistance();
    servo.write(90);

    return value;
}

void moveStop()
{

    Serial.println("Move Stop");

    motor2.run(RELEASE);
    // motor3.run(RELEASE);
}

void moveForward()
{
    Serial.println("Move Forward");

    if (!goesForward)
    {
        goesForward = true;

        motor2.run(FORWARD);
        // motor3.run(FORWARD);

        for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
        {

            motor2.setSpeed(speedSet);
            // motor3.setSpeed(speedSet);
            delay(5);
        }
    }
}

void moveBackward()
{
    Serial.println("Move Backward");

    goesForward = false;

    motor2.run(BACKWARD);
    // motor3.run(BACKWARD);

    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
    {

        motor2.setSpeed(speedSet);
        // motor3.setSpeed(speedSet);
        delay(5);
    }
}

void turnRight()
{
    Serial.println("Turn Right");

    motor2.run(FORWARD);
    // motor3.run(BACKWARD);
    delay(500);
    motor2.run(FORWARD);
    // motor3.run(FORWARD);
}

void turnLeft()
{
    Serial.println("Turn left");

    motor2.run(BACKWARD);
    // motor3.run(FORWARD);
    delay(500);
    motor2.run(FORWARD);
    // motor3.run(FORWARD);
}
