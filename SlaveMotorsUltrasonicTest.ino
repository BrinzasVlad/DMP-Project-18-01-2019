#include <Wire.h>
#include <Servo.h>

/*
 * Commands table:
 */
#define STOP 0
#define FWD 1
#define BACK 2
#define LT 3
#define RT 4

//Slave address at 9
#define SLAVE 9

// Global variable for the command we read via I2C
volatile int command = 0;

// Global variable for the distance detected via ultrasonics
volatile int distance = 100;
int duration; //This is a variable to be used in computing distance

//If the distance is this or lower, STOP
#define limitDistance 15


// Pins of motor 1
#define mpin00 5
#define mpin01 6

// Pins of motor 2
#define mpin10 3
#define mpin11 11

Servo srv;
#define servoPin 8


// Pins for ultrasonic sensor
#define trigPin 10
#define echoPin 9

void setup() {
    Serial.begin(9600);
    
    // configuration of motor pins as output, initially 0
    digitalWrite(mpin00, 0);
    digitalWrite(mpin01, 0);
    digitalWrite(mpin10, 0);
    digitalWrite(mpin11, 0);
    
    pinMode (mpin00, OUTPUT);
    pinMode (mpin01, OUTPUT);
    pinMode (mpin10, OUTPUT);
    pinMode (mpin11, OUTPUT);
    
    // LED pin
    pinMode(13, OUTPUT);

    // Set servo so that we're looking forward (at least in theory)
    srv.attach(servoPin);
    srv.write(90); // This should be in the middle; change if needed, I s'pose
    srv.detach();

    
    // LED warning code for things being about to happen
    // Slow blink
    for (int i=0; i<10; i++) //2 seconds
    {
        digitalWrite(13, 1);
        delay(200);
        digitalWrite(13, 0);
        delay(200);
    }
    
    // Fast blink. Remove the USB cable!!!!
    for (int i=0; i<10; i++) //1 second
    {
        digitalWrite(13, 1);
        delay(100);
        digitalWrite(13, 0);
        delay(100);
    }
    
    digitalWrite(13, 1);

    

    // Now open up as slave and start receiving commands
    Wire.begin(SLAVE);
    Wire.onReceive(receiveEvent);
}

// Function to control a motor
// Input: pins m1 and m2, direction and speed
void StartMotor (int m1, int m2, int forward, int speed)
{

    if (speed==0) // stop
    {
        digitalWrite(m1, 0);
        digitalWrite(m2, 0);
    }
    else
    {
        if (forward)
        {
            digitalWrite(m2, 0);
            analogWrite (m1, speed); // use PWM
        }
        else
        {
            digitalWrite(m1, 0);
            analogWrite(m2, speed);
        }
    }
}

void receiveEvent(int bytes) {
    command = Wire.read(); //read I2C received command
    switch (command) {
        case FWD:
            Serial.print("FWD\n");
            Serial.print("\nDistance was ");
            Serial.print(distance);
            Serial.print("\n\n");
			//if (distance <= limitDistance) { //Stop when commanded to move into obstacles
			//	command = STOP;
			//	StartMotor (mpin00, mpin01, 0, 0);
			//	StartMotor (mpin10, mpin11, 0, 0);
			//}
            //else {
                StartMotor (mpin00, mpin01, 1, 128);
                StartMotor (mpin10, mpin11, 1, 128);
            //}
            break;
        case BACK:
            Serial.print("BACK\n");
            StartMotor (mpin00, mpin01, 0, 128);
            StartMotor (mpin10, mpin11, 0, 128);
            break;
        case LT:
            Serial.print("LEFT\n");
            StartMotor (mpin00, mpin01, 0, 128);
            StartMotor (mpin10, mpin11, 1, 128);
            break;
        case RT:
            Serial.print("RIGHT\n");
            StartMotor (mpin00, mpin01, 1, 128);
            StartMotor (mpin10, mpin11, 0, 128);
            break;
        default: //STOP is default behaviour
            Serial.print("STOP\n");
            StartMotor (mpin00, mpin01, 0, 0);
            StartMotor (mpin10, mpin11, 0, 0);
            break;
    }
}

void loop() {    
    // Clears the trigPin for ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds (launch sensing)
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance = duration*0.034/2;
    
    Serial.print("\nDistance was ");
    Serial.print(distance);
    Serial.print("\n\n");

    // If the distance is too short, STOP (to prevent collisions)
    if(distance <= limitDistance
       && command == FWD
       && distance != 0) {
        command = STOP;
        StartMotor (mpin00, mpin01, 0, 0);
        StartMotor (mpin10, mpin11, 0, 0);
    }
}
