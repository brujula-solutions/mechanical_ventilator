
/**
 * 
 * IMPORTANT: Only enable the serial writing code for debbuging. This will affect and block the correct functioning of the motor.
 */

const int potenciometerPin = 14;    // select the input pin for the potentiometer  This is the A0 on the Arduino board
int val = 0;                        // variable to store the value coming from the potenciometer

const int stepPin = 12;             //pin to pulse for steps
const int dirPin = 11;              //pin to change step direction
const int stepResolution = 8;       // Motor step resolution

// Speed function
const int pulsesForFullLoop = 800;  // Full:200, Half: 400, Quarter: 800, Eighth: 1600, Sixteenth: 3200
float slope = 1;
int pulseDelay = 1;

void setup(){
  //Start serial Comms to see the potenciometer value
  //Serial.begin(9600);

  slope = (float)(5.0/341.0); // This is the function to control the speed. For a zero on the potenciometer gives 15, for 1023 gives 30
  
  //set pins as outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepResolution, OUTPUT);

  digitalWrite(dirPin, LOW);           // Enables the motor to move in a particular direction
  digitalWrite(stepResolution, HIGH);  // This pin only puts a HIGH into an output to allow adjusting the step resolution
}

void loop(){
  stepperFWD();
}

//change the stepper direction to forward
void stepperFWD() {
  // Makes 200 pulses for making one full cycle rotation
  pulseDelay = calculatePulseDelay();
  digitalWrite(stepPin, HIGH); 
  delayMicroseconds(pulseDelay); 
  
  digitalWrite(stepPin, LOW); 
  delayMicroseconds(pulseDelay);
}

int calculatePulseDelay() {
  // We read the value of the potenciometer
  val = analogRead(potenciometerPin);
  //Print the potenciometer value in the serial monitor
  //Serial.print("Potenciometer value is: ");
  //Serial.println(val);
  
  float freqPerMin = (int) ((slope * val) + 15);    // Freq loops / minute = (5.0/341.0) x + 15 => Transforms potenciometer value into loops/minute. The range go from 15 to 30 loops
  float timePerLoop = (60000000 / freqPerMin);                     // 1 minute (in microseconds) / Number of loops per minute = time for each loop
  float motorDelayPerPulse = (timePerLoop / pulsesForFullLoop);    // The controller needs to send N number of pulses to the motor driver to do an entire loop

/*
  Only enable this code for debbuging. This will affect and block the correct functioning of the motor
  Serial.print("Slope: ");
  Serial.print(slope);
  Serial.print(" - Motor delay per pulse: ");
  Serial.print(motorDelayPerPulse);
  Serial.print(" - Frecuency: ");
  Serial.print(freqPerMin);
  Serial.println(" frecuency/min");
*/
  int halfPulse =  (int) (motorDelayPerPulse / 2);                 // The controller simulates the pulse by sending a HIGH-LOW signal. The delay is divided in 2
  return halfPulse;
}
