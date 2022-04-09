#include <Servo.h>
#include <AFMotor.h>

const int PIN_SERVO = 10;
const int PIN_TRIG = A0;
const int PIN_ECHO = A2;
const int PIN_BUZZER = 5;

#define MAX_DISTANCE 200
#define MAX_SPEED 150 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

AF_DCMotor leftFront(2);
AF_DCMotor rightFront(3);

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

    leftFront.setSpeed(MAX_SPEED);
    rightFront.setSpeed(MAX_SPEED);

    // Make sure all wheels are stopped
    leftFront.run(RELEASE);
    rightFront.run(RELEASE);
}

void loop()
{
    int distanceLeft = 0;
    int distanceRight = 0;

    ultraSonicDistance = lookForward();

    if (ultraSonicDistance < 15)
    {
        moveStop();

        tone(PIN_BUZZER, 1500, 300);
        delay(500);
        tone(PIN_BUZZER, 500, 300);
        delay(500);
        tone(PIN_BUZZER, 1500, 300);
        delay(300);

        lookRight(); // call this method to avoid zero return value
        delay(300);
        distanceRight = lookRight();
        delay(300);
        lookLeft(); // call this method to avoid zero return value
        delay(300);
        distanceLeft = lookLeft();

        if (distanceLeft < 15 && distanceRight < 15)
        {
            // move backward
            moveBackward();
            delay(1000);
        }
        else
        {
            if (distanceLeft > distanceRight)
            {
                // move to left
                Serial.println("Move left");
                turnLeft();
            }
            else
            {
                // move to right
                Serial.println("move right");
                turnRight();
            }
        }
    }
    else
    {
        // move forward
        // Serial.println("move forward");
        moveForward();
    }

    lookForward(); // call this method to avoid zero return value
}

void moveForward()
{
    leftFront.run(FORWARD);
    rightFront.run(FORWARD);
}

void moveStop()
{
    leftFront.run(RELEASE);
    rightFront.run(RELEASE);
}

void moveBackward()
{
    leftFront.run(BACKWARD);
    rightFront.run(BACKWARD);
}

void turnLeft()
{
    leftFront.run(BACKWARD);
    rightFront.run(FORWARD);
    delay(300);
    moveStop();
}

void turnRight()
{
    leftFront.run(FORWARD);
    rightFront.run(BACKWARD);
    delay(300);
    moveStop();
}

int lookForward()
{
    servo.write(90);

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

int lookLeft()
{
    servo.write(180);
    // Clears the trigPin condition
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    int duration = pulseIn(PIN_ECHO, HIGH);
    // Calculating the distance
    return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

int lookRight()
{
    servo.write(0);

    // // Clears the trigPin condition
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    int duration = pulseIn(PIN_ECHO, HIGH);
    // Calculating the distance
    return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}
