//Automation in action - Establishing autonomous vehicle networking systems
//This is the code for the project, please refer to the electrical schematic to get better idea about the circuit


const int pnp1 = 2;
const int pnp2 = 3;
const int pnp3 = 4;
const int pnp4 = 5;
const int npn1 = 7;
const int npn2 = 6;
const int npn3 = 8;
const int npn4 = 9;

const int throttlepin = 53;
const int steeringpin = 52;
const int modepin = 51;


const int leftfrontechopin = 22;
const int leftfronttrigpin = 23;
const int frontleftsensorechopin = 24;
const int frontleftsensortrigpin = 25;
const int frontrightsensorechopin = 26;
const int frontrightsensortrigpin = 27;
const int rightfrontechopin = 28;
const int rightfronttrigpin = 29;
const int frontechopin = 30;
const int fronttrigpin = 31;

const int irleftfrontpin = 48;
const int irfrontleftpin = 46;
const int irfrontrightpin = 47;
const int irrightfrontpin = 49;
const int irbackleftpin = 42;
const int irbackrightpin = 43;
const int irdownleftpin = 44;
const int irdownrightpin = 45;


int rotation;
int mode = 1;

int throttleval;
int steeringval;
int modeval;

unsigned int leftfrontdistance = 50000;
unsigned int frontleftsensordistance = 50000;
unsigned int frontrightsensordistance = 50000;
unsigned int rightfrontdistance = 50000;
unsigned int frontdistance = 50000;
unsigned int duration;

unsigned int irleftfrontstatus = 0;
unsigned int irrightfrontstatus = 0;
unsigned int irfrontleftstatus = 0;
unsigned int irfrontrightstatus = 0;
unsigned int irbackleftstatus = 0;
unsigned int irbackrightstatus = 0;
unsigned int irdownleftstatus = 0;
unsigned int irdownrightstatus = 0;


int throttlemode = 0;
int steeringmode = 0;

int newmodechange = 0;

int minleftsensors = 0;
int minrightsensors = 0;

int motorspeed = 255;
int additionalsensitivity = 0;

const char voltagepin = A1;

float batteryvoltage;



void setup() {
  Serial.begin(2000000); // Starts the serial communication

  pinMode(pnp1, OUTPUT);
  pinMode(pnp2, OUTPUT);
  pinMode(pnp3, OUTPUT);
  pinMode(pnp4, OUTPUT);
  pinMode(npn1, OUTPUT);
  pinMode(npn2, OUTPUT);
  pinMode(npn3, OUTPUT);
  pinMode(npn4, OUTPUT);

  pinMode(throttlepin, INPUT_PULLUP);
  pinMode(steeringpin, INPUT_PULLUP);
  pinMode(modepin, INPUT_PULLUP);

  pinMode(leftfronttrigpin, OUTPUT);
  pinMode(leftfrontechopin, INPUT_PULLUP);
  pinMode(frontleftsensortrigpin, OUTPUT);
  pinMode(frontleftsensorechopin, INPUT_PULLUP);
  pinMode(frontrightsensortrigpin, OUTPUT);
  pinMode(frontrightsensorechopin, INPUT_PULLUP);
  pinMode(rightfronttrigpin, OUTPUT);
  pinMode(rightfrontechopin, INPUT_PULLUP);
  pinMode(fronttrigpin, OUTPUT);
  pinMode(frontechopin, INPUT_PULLUP);

  pinMode(irleftfrontpin, INPUT_PULLUP);
  pinMode(irrightfrontpin, INPUT_PULLUP);
  pinMode(irfrontleftpin, INPUT_PULLUP);
  pinMode(irfrontrightpin, INPUT_PULLUP);
  pinMode(irbackleftpin, INPUT_PULLUP);
  pinMode(irbackrightpin, INPUT_PULLUP);
  pinMode(irdownleftpin, INPUT_PULLUP);
  pinMode(irdownrightpin, INPUT_PULLUP);

  pinMode(13, OUTPUT);

}

void loop() {


  throttleval = pulseIn(throttlepin, HIGH);
  steeringval = pulseIn(steeringpin, HIGH);
  modeval = pulseIn(modepin, HIGH);

  if (modeval < 1250) mode = 1;
  else if ((modeval > 1250) and (modeval < 1750)) {
    if (newmodechange == 1) {
      steeringmode = 0;
      throttlemode = 0;
      newmodechange = 0;
    }
    mode = 2;
  }
  else if (modeval > 1750) mode = 3;

  batteryvoltage = ((analogRead(voltagepin) * (5.00 / 1023.00)) * 2.00) - 0.57;
  if (batteryvoltage < 0) batteryvoltage = 0;
  additionalsensitivity = map(motorspeed, 0, 255, 0, 10) - map(batteryvoltage, 6.60, 8.40, 10, 0);
  if (additionalsensitivity < -15) additionalsensitivity = -15;
  if (batteryvoltage > 6.60) {
    digitalWrite(13, HIGH);
    if (mode == 1) {
      digitalWrite(13, HIGH);
      Serial.print("Throttle:");
      Serial.print(pulseIn(throttlepin, HIGH));
      Serial.print(" | Steering:");
      Serial.print(pulseIn(steeringpin, HIGH));
      Serial.print(" | Mode:");
      Serial.println(pulseIn(modepin, HIGH));


      if ((throttleval > 1400) and (throttleval < 1600) and (steeringval > 1400) and (steeringval < 1600)) rotation = 0; // Stop
      else if ((throttleval > 1600) and (steeringval > 1400) and (steeringval < 1600)) rotation = 1; // Forward
      else if ((throttleval < 1400) and (steeringval > 1400) and (steeringval < 1600)) rotation = 2; // Backward
      else if ((throttleval > 1400) and (throttleval < 1600) and (steeringval > 1600)) rotation = 3; // Right
      else if ((throttleval > 1400) and (throttleval < 1600) and (steeringval < 1400)) rotation = 4; // Left
      else if ((throttleval > 1600) and (steeringval > 1600)) rotation = 5; //Forward Right
      else if ((throttleval > 1600) and (steeringval < 1400)) rotation = 6; //Forward Left
      else if ((throttleval < 1400) and (steeringval > 1600)) rotation = 7; //Backward Right
      else if ((throttleval < 1400) and (steeringval < 1400)) rotation = 8; //Backward Left
      if (rotation == 0) { //Stop
        digitalWrite(npn1, LOW);
        digitalWrite(npn2, LOW);
        digitalWrite(npn3, LOW);
        digitalWrite(npn4, LOW);
        digitalWrite(pnp1, LOW);
        digitalWrite(pnp2, LOW);
        digitalWrite(pnp3, LOW);
        digitalWrite(pnp4, LOW);
      }

      if (rotation == 1) { //Forward
        //Off
        digitalWrite(npn1, LOW);
        digitalWrite(npn3, LOW);
        digitalWrite(pnp2, LOW);
        digitalWrite(pnp4, LOW);
        //On
        analogWrite(npn2, motorspeed);
        analogWrite(npn4, motorspeed);
        digitalWrite(pnp1, HIGH);
        digitalWrite(pnp3, HIGH);
      }
      if (rotation == 2) { //Backward
        //Off
        digitalWrite(npn2, LOW);
        digitalWrite(npn4, LOW);
        digitalWrite(pnp1, LOW);
        digitalWrite(pnp3, LOW);
        //On
        analogWrite(npn1, motorspeed);
        analogWrite(npn3, motorspeed);
        digitalWrite(pnp2, HIGH);
        digitalWrite(pnp4, HIGH);
      }
      if (rotation == 3) { //Right
        //Off
        digitalWrite(npn1, LOW);
        digitalWrite(npn4, LOW);
        digitalWrite(pnp2, LOW);
        digitalWrite(pnp3, LOW);
        //On
        analogWrite(npn2, motorspeed);
        analogWrite(npn3, motorspeed);
        digitalWrite(pnp1, HIGH);
        digitalWrite(pnp4, HIGH);
      }
      if (rotation == 4) { //Left
        //Off
        digitalWrite(npn2, LOW);
        digitalWrite(npn3, LOW);
        digitalWrite(pnp1, LOW);
        digitalWrite(pnp4, LOW);
        //On
        analogWrite(npn1, motorspeed);
        analogWrite(npn4, motorspeed);
        digitalWrite(pnp2, HIGH);
        digitalWrite(pnp3, HIGH);
      }
      if (rotation == 5) { //Forward Right
        //Off
        digitalWrite(npn1, LOW);
        digitalWrite(npn3, LOW);
        digitalWrite(pnp2, LOW);
        digitalWrite(pnp4, LOW);
        //On
        analogWrite(npn2, motorspeed);
        analogWrite(npn4, motorspeed / 4);
        digitalWrite(pnp1, HIGH);
        digitalWrite(pnp3, HIGH);
      }
      if (rotation == 6) { //Forward Left
        //Off
        digitalWrite(npn1, LOW);
        digitalWrite(npn3, LOW);
        digitalWrite(pnp2, LOW);
        digitalWrite(pnp4, LOW);
        //On
        analogWrite(npn2, motorspeed / 4);
        analogWrite(npn4, motorspeed);
        digitalWrite(pnp1, HIGH);
        digitalWrite(pnp3, HIGH);
      }
      if (rotation == 7) { //Backward Right
        //Off
        digitalWrite(npn2, LOW);
        digitalWrite(npn4, LOW);
        digitalWrite(pnp1, LOW);
        digitalWrite(pnp3, LOW);
        //On
        analogWrite(npn1, motorspeed);
        analogWrite(npn3, motorspeed / 4);
        digitalWrite(pnp2, HIGH);
        digitalWrite(pnp4, HIGH);
      }
      if (rotation == 8) { //Backward Left
        //Off
        digitalWrite(npn2, LOW);
        digitalWrite(npn4, LOW);
        digitalWrite(pnp1, LOW);
        digitalWrite(pnp3, LOW);
        //On
        analogWrite(npn1, 50);
        analogWrite(npn3, motorspeed);
        digitalWrite(pnp2, HIGH);
        digitalWrite(pnp4, HIGH);
      }
    }

    if (mode == 2) {
      if (throttlemode == 0) { //Diagnostics mode
        if (steeringmode == 0) { //Print remote control parameters
          digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, LOW);
          duration = pulseIn(leftfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            leftfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, LOW);
          duration = pulseIn(rightfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            rightfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, LOW);
          duration = pulseIn(frontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontdistance = duration * 0.034 / 2;
          }


          irleftfrontstatus = digitalRead(irleftfrontpin);
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          if (leftfrontdistance == 0) leftfrontdistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (rightfrontdistance == 0) rightfrontdistance = 50000;
          if (frontdistance == 0) frontdistance = 50000;

          throttleval = pulseIn(throttlepin, HIGH);
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          Serial.print("Speed: ");
          Serial.print(motorspeed);
          Serial.print(" | Voltage: ");
          Serial.print(batteryvoltage);
          Serial.print("V | Sensitivity: ");
          Serial.print(additionalsensitivity);
          Serial.print(" | Throttle: ");
          Serial.print(throttleval);
          Serial.print(" | Steering: ");
          Serial.print(steeringval);
          Serial.print(" | Mode: ");
          Serial.println(modeval);


          if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
          else if (throttleval < 1400) throttlemode = 1;
          else if (throttleval > 1600) throttlemode = 2;

          if ((steeringval > 1400) and (steeringval < 1600)) steeringmode = 0; //Check steering mode
          else if (steeringval < 1400) steeringmode = 1;
          else if (steeringval > 1600) steeringmode = 2;
        }

        if (steeringmode == 1) { //Print ultrasonic sensors parameters
          digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, LOW);
          duration = pulseIn(leftfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            leftfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, LOW);
          duration = pulseIn(rightfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            rightfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, LOW);
          duration = pulseIn(frontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontdistance = duration * 0.034 / 2;
          }


          irleftfrontstatus = digitalRead(irleftfrontpin);
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          if (leftfrontdistance == 0) leftfrontdistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (rightfrontdistance == 0) rightfrontdistance = 50000;
          if (frontdistance == 0) frontdistance = 50000;

          throttleval = pulseIn(throttlepin, HIGH);
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          Serial.print("Left Front: ");
          Serial.print(leftfrontdistance);
          Serial.print(" | Front Left: ");
          Serial.print(frontleftsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Right Front: ");
          Serial.print(rightfrontdistance);
          Serial.print(" | Front: ");
          Serial.println(frontdistance);

          if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
          else if (throttleval < 1400) throttlemode = 1;
          else if (throttleval > 1600) throttlemode = 2;

          if ((steeringval > 1400) and (steeringval < 1600)) steeringmode = 0; //Check steering mode
          else if (steeringval < 1400) steeringmode = 1;
          else if (steeringval > 1600) steeringmode = 2;
        }

        if (steeringmode == 2) { //Print IR sensor parameters
          digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, LOW);
          duration = pulseIn(leftfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            leftfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, LOW);
          duration = pulseIn(rightfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            rightfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, LOW);
          duration = pulseIn(frontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontdistance = duration * 0.034 / 2;
          }


          irleftfrontstatus = digitalRead(irleftfrontpin);
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          if (leftfrontdistance == 0) leftfrontdistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (rightfrontdistance == 0) rightfrontdistance = 50000;
          if (frontdistance == 0) frontdistance = 50000;

          throttleval = pulseIn(throttlepin, HIGH);
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          Serial.print("Left Front IR: ");
          Serial.print(irleftfrontstatus);
          Serial.print(" | Front Left IR: ");
          Serial.print(irfrontleftstatus);
          Serial.print(" | Front Right IR: ");
          Serial.print(irfrontrightstatus);
          Serial.print(" | Right Front IR: ");
          Serial.print(irrightfrontstatus);
          Serial.print(" | Back Left IR: ");
          Serial.print(irbackleftstatus);
          Serial.print(" | Back Right IR: ");
          Serial.print(irbackrightstatus);
          Serial.print(" | Down Left IR: ");
          Serial.print(irdownleftstatus);
          Serial.print(" | Down Right IR: ");
          Serial.println(irdownrightstatus);

          if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
          else if (throttleval < 1400) throttlemode = 1;
          else if (throttleval > 1600) throttlemode = 2;

          if ((steeringval > 1400) and (steeringval < 1600)) steeringmode = 0; //Check steering mode
          else if (steeringval < 1400) steeringmode = 1;
          else if (steeringval > 1600) steeringmode = 2;
        }
      }

      if (throttlemode == 1) {
        digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
        delayMicroseconds(5);
        digitalWrite(leftfronttrigpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(leftfronttrigpin, LOW);
        duration = pulseIn(leftfrontechopin, HIGH, 20000);
        if ((duration * 0.034 / 2) < 300) {
          leftfrontdistance = duration * 0.034 / 2;
        }
        digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
        delayMicroseconds(5);
        digitalWrite(frontleftsensortrigpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(frontleftsensortrigpin, LOW);
        duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
        if ((duration * 0.034 / 2) < 300) {
          frontleftsensordistance = duration * 0.034 / 2;
        }
        digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
        delayMicroseconds(5);
        digitalWrite(frontrightsensortrigpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(frontrightsensortrigpin, LOW);
        duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
        if ((duration * 0.034 / 2) < 300) {
          frontrightsensordistance = duration * 0.034 / 2;
        }
        digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
        delayMicroseconds(5);
        digitalWrite(rightfronttrigpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(rightfronttrigpin, LOW);
        duration = pulseIn(rightfrontechopin, HIGH, 20000);
        if ((duration * 0.034 / 2) < 300) {
          rightfrontdistance = duration * 0.034 / 2;
        }
        digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
        delayMicroseconds(5);
        digitalWrite(fronttrigpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(fronttrigpin, LOW);
        duration = pulseIn(frontechopin, HIGH, 20000);
        if ((duration * 0.034 / 2) < 300) {
          frontdistance = duration * 0.034 / 2;
        }

        irleftfrontstatus = digitalRead(irleftfrontpin);
        irrightfrontstatus = digitalRead(irrightfrontpin);
        irfrontleftstatus = digitalRead(irfrontleftpin);
        irfrontrightstatus = digitalRead(irfrontrightpin);
        irbackleftstatus = digitalRead(irbackleftpin);
        irbackrightstatus = digitalRead(irbackrightpin);
        irdownleftstatus = digitalRead(irdownleftpin);
        irdownrightstatus = digitalRead(irdownrightpin);

        if (leftfrontdistance == 0) leftfrontdistance = 50000;
        if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
        if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
        if (rightfrontdistance == 0) rightfrontdistance = 50000;
        if (frontdistance == 0) frontdistance = 50000;

        throttleval = pulseIn(throttlepin, HIGH);
        steeringval = pulseIn(steeringpin, HIGH);
        modeval = pulseIn(modepin, HIGH);

        if ((irdownleftstatus == 0) and (irdownrightstatus == 0) and (mode == 2)) {
          if ((leftfrontdistance > (20 + additionalsensitivity)) and (frontleftsensordistance > (20 + additionalsensitivity)) and (frontrightsensordistance > (20 + additionalsensitivity)) and (rightfrontdistance > (20 + additionalsensitivity)) and (frontdistance > (25 + additionalsensitivity)) and (irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1) and (mode == 2)) {
            if (((leftfrontdistance < 40) or (frontleftsensordistance < 40) or (frontrightsensordistance < 40) or (rightfrontdistance < 40) or (frontleftsensordistance < 50)) and (mode == 2)) {
              if (minleftsensors < minrightsensors) {
                //Forward Right
                //Off
                digitalWrite(npn1, LOW);
                digitalWrite(npn3, LOW);
                digitalWrite(pnp2, LOW);
                digitalWrite(pnp4, LOW);
                //On
                analogWrite(npn2, motorspeed);
                analogWrite(npn4, motorspeed / 4);
                digitalWrite(pnp1, HIGH);
                digitalWrite(pnp3, HIGH);
              } else if (minleftsensors > minrightsensors) {
                //Forward Left
                //Off
                digitalWrite(npn1, LOW);
                digitalWrite(npn3, LOW);
                digitalWrite(pnp2, LOW);
                digitalWrite(pnp4, LOW);
                //On
                analogWrite(npn2, motorspeed / 4);
                analogWrite(npn4, motorspeed);
                digitalWrite(pnp1, HIGH);
                digitalWrite(pnp3, HIGH);
              } else {
                //Forward
                //Off
                digitalWrite(npn1, LOW);
                digitalWrite(npn3, LOW);
                digitalWrite(pnp2, LOW);
                digitalWrite(pnp4, LOW);
                //On
                analogWrite(npn2, motorspeed);
                analogWrite(npn4, motorspeed);
                digitalWrite(pnp1, HIGH);
                digitalWrite(pnp3, HIGH);
              }
              minleftsensors = min(leftfrontdistance, frontleftsensordistance);
              minrightsensors = min(rightfrontdistance, frontrightsensordistance);
            } else {
              //Forward
              //Off
              digitalWrite(npn1, LOW);
              digitalWrite(npn3, LOW);
              digitalWrite(pnp2, LOW);
              digitalWrite(pnp4, LOW);
              //On
              analogWrite(npn2, motorspeed);
              analogWrite(npn4, motorspeed);
              digitalWrite(pnp1, HIGH);
              digitalWrite(pnp3, HIGH);
            }

            digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
            delayMicroseconds(5);
            digitalWrite(leftfronttrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(leftfronttrigpin, LOW);
            duration = pulseIn(leftfrontechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              leftfrontdistance = duration * 0.034 / 2;
            }
            digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
            delayMicroseconds(5);
            digitalWrite(frontleftsensortrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(frontleftsensortrigpin, LOW);
            duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              frontleftsensordistance = duration * 0.034 / 2;
            }
            digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
            delayMicroseconds(5);
            digitalWrite(frontrightsensortrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(frontrightsensortrigpin, LOW);
            duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              frontrightsensordistance = duration * 0.034 / 2;
            }
            digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
            delayMicroseconds(5);
            digitalWrite(rightfronttrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(rightfronttrigpin, LOW);
            duration = pulseIn(rightfrontechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              rightfrontdistance = duration * 0.034 / 2;
            }
            digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
            delayMicroseconds(5);
            digitalWrite(fronttrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(fronttrigpin, LOW);
            duration = pulseIn(frontechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              frontdistance = duration * 0.034 / 2;
            }
            if (leftfrontdistance == 0) leftfrontdistance = 50000;
            if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
            if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
            if (rightfrontdistance == 0) rightfrontdistance = 50000;
            if (frontdistance == 0) frontdistance = 50000;

            irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
            irrightfrontstatus = digitalRead(irrightfrontpin);
            irfrontleftstatus = digitalRead(irfrontleftpin);
            irfrontrightstatus = digitalRead(irfrontrightpin);
            irbackleftstatus = digitalRead(irbackleftpin);
            irbackrightstatus = digitalRead(irbackrightpin);
            irdownleftstatus = digitalRead(irdownleftpin);
            irdownrightstatus = digitalRead(irdownrightpin);

            throttleval = pulseIn(throttlepin, HIGH); //Check mode
            steeringval = pulseIn(steeringpin, HIGH);
            modeval = pulseIn(modepin, HIGH);
            if (modeval < 1250) {
              mode = 1;
              newmodechange = 1;
            }
            else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
            else if (modeval > 1750) {
              mode = 3;
            }

          } else if ((irleftfrontstatus == 0) and (irrightfrontstatus == 0) and (irfrontleftstatus == 0) and (irfrontrightstatus == 0) and (mode == 2)) {
            Serial.println("Action 2");
            digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
            delayMicroseconds(5);
            digitalWrite(leftfronttrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(leftfronttrigpin, LOW);
            duration = pulseIn(leftfrontechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              leftfrontdistance = duration * 0.034 / 2;
            }
            digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
            delayMicroseconds(5);
            digitalWrite(frontleftsensortrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(frontleftsensortrigpin, LOW);
            duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              frontleftsensordistance = duration * 0.034 / 2;
            }
            digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
            delayMicroseconds(5);
            digitalWrite(frontrightsensortrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(frontrightsensortrigpin, LOW);
            duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              frontrightsensordistance = duration * 0.034 / 2;
            }
            digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
            delayMicroseconds(5);
            digitalWrite(rightfronttrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(rightfronttrigpin, LOW);
            duration = pulseIn(rightfrontechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              rightfrontdistance = duration * 0.034 / 2;
            }
            digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
            delayMicroseconds(5);
            digitalWrite(fronttrigpin, HIGH);
            delayMicroseconds(5);
            digitalWrite(fronttrigpin, LOW);
            duration = pulseIn(frontechopin, HIGH, 20000);
            if ((duration * 0.034 / 2) < 300) {
              frontdistance = duration * 0.034 / 2;
            }
            if (leftfrontdistance == 0) leftfrontdistance = 0;
            if (frontleftsensordistance == 0) frontleftsensordistance = 0;
            if (frontrightsensordistance == 0) frontrightsensordistance = 0;
            if (rightfrontdistance == 0) rightfrontdistance = 0;
            if (frontdistance == 0) frontdistance = 0;

            if (!(((irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1)) and (irbackleftstatus == 1) and (irbackrightstatus == 1)) and (mode == 2)) {
              //Backward
              //Off
              digitalWrite(npn2, LOW);
              digitalWrite(npn4, LOW);
              digitalWrite(pnp1, LOW);
              digitalWrite(pnp3, LOW);
              //On
              analogWrite(npn1, motorspeed);
              analogWrite(npn3, motorspeed);
              digitalWrite(pnp2, HIGH);
              digitalWrite(pnp4, HIGH);

              irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
              irrightfrontstatus = digitalRead(irrightfrontpin);
              irfrontleftstatus = digitalRead(irfrontleftpin);
              irfrontrightstatus = digitalRead(irfrontrightpin);
              irbackleftstatus = digitalRead(irbackleftpin);
              irbackrightstatus = digitalRead(irbackrightpin);
              irdownleftstatus = digitalRead(irdownleftpin);
              irdownrightstatus = digitalRead(irdownrightpin);

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }
            if (((leftfrontdistance + frontleftsensordistance) / 2) > ((rightfrontdistance + frontrightsensordistance) / 2)) {
              Serial.println("Action 3");
              if (!((leftfrontdistance > (20 + additionalsensitivity)) and (frontleftsensordistance > (20 + additionalsensitivity)) and (frontrightsensordistance > (20 + additionalsensitivity)) and (rightfrontdistance > (20 + additionalsensitivity)) and (frontdistance > (25 + additionalsensitivity))) and (mode == 2)) {
                //Left
                //Off
                digitalWrite(npn2, LOW);
                digitalWrite(npn3, LOW);
                digitalWrite(pnp1, LOW);
                digitalWrite(pnp4, LOW);
                //On
                analogWrite(npn1, motorspeed);
                analogWrite(npn4, motorspeed);
                digitalWrite(pnp2, HIGH);
                digitalWrite(pnp3, HIGH);

                digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
                delayMicroseconds(5);
                digitalWrite(leftfronttrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(leftfronttrigpin, LOW);
                duration = pulseIn(leftfrontechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  leftfrontdistance = duration * 0.034 / 2;
                }
                digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
                delayMicroseconds(5);
                digitalWrite(frontleftsensortrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(frontleftsensortrigpin, LOW);
                duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  frontleftsensordistance = duration * 0.034 / 2;
                }
                digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
                delayMicroseconds(5);
                digitalWrite(frontrightsensortrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(frontrightsensortrigpin, LOW);
                duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  frontrightsensordistance = duration * 0.034 / 2;
                }
                digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
                delayMicroseconds(5);
                digitalWrite(rightfronttrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(rightfronttrigpin, LOW);
                duration = pulseIn(rightfrontechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  rightfrontdistance = duration * 0.034 / 2;
                }
                digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
                delayMicroseconds(5);
                digitalWrite(fronttrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(fronttrigpin, LOW);
                duration = pulseIn(frontechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  frontdistance = duration * 0.034 / 2;
                }
                if (leftfrontdistance == 0) leftfrontdistance = 50000;
                if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
                if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
                if (rightfrontdistance == 0) rightfrontdistance = 50000;
                if (frontdistance == 0) frontdistance = 50000;

                throttleval = pulseIn(throttlepin, HIGH); //Check mode
                steeringval = pulseIn(steeringpin, HIGH);
                modeval = pulseIn(modepin, HIGH);
                if (modeval < 1250) {
                  mode = 1;
                  newmodechange = 1;
                }
                else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
                else if (modeval > 1750) {
                  mode = 3;
                  newmodechange = 1;
                }
              }
            } else if (!((leftfrontdistance > (20 + additionalsensitivity)) and (frontleftsensordistance > (20 + additionalsensitivity)) and (frontrightsensordistance > (20 + additionalsensitivity)) and (rightfrontdistance > (20 + additionalsensitivity)) and (frontdistance > (25 + additionalsensitivity))) and (mode == 2)) {
              //Right
              //Off
              digitalWrite(npn1, LOW);
              digitalWrite(npn4, LOW);
              digitalWrite(pnp2, LOW);
              digitalWrite(pnp3, LOW);
              //On
              analogWrite(npn2, motorspeed);
              analogWrite(npn3, motorspeed);
              digitalWrite(pnp1, HIGH);
              digitalWrite(pnp4, HIGH);

              digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
              delayMicroseconds(5);
              digitalWrite(leftfronttrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(leftfronttrigpin, LOW);
              duration = pulseIn(leftfrontechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                leftfrontdistance = duration * 0.034 / 2;
              }
              digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
              delayMicroseconds(5);
              digitalWrite(frontleftsensortrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(frontleftsensortrigpin, LOW);
              duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                frontleftsensordistance = duration * 0.034 / 2;
              }
              digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
              delayMicroseconds(5);
              digitalWrite(frontrightsensortrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(frontrightsensortrigpin, LOW);
              duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                frontrightsensordistance = duration * 0.034 / 2;
              }
              digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
              delayMicroseconds(5);
              digitalWrite(rightfronttrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(rightfronttrigpin, LOW);
              duration = pulseIn(rightfrontechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                rightfrontdistance = duration * 0.034 / 2;
              }
              digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
              delayMicroseconds(5);
              digitalWrite(fronttrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(fronttrigpin, LOW);
              duration = pulseIn(frontechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                frontdistance = duration * 0.034 / 2;
              }
              if (leftfrontdistance == 0) leftfrontdistance = 50000;
              if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
              if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
              if (rightfrontdistance == 0) rightfrontdistance = 50000;
              if (frontdistance == 0) frontdistance = 50000;

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }
          } else if (((irleftfrontstatus == 0) or (irfrontleftstatus == 0)) and (irfrontrightstatus == 1) and (irrightfrontstatus == 1) and (mode == 2)) {
            Serial.println("Action 4");
            if (!((irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1)) and (mode == 2)) {
              Serial.println("Action 6");
              //Right
              //Off
              digitalWrite(npn1, LOW);
              digitalWrite(npn4, LOW);
              digitalWrite(pnp2, LOW);
              digitalWrite(pnp3, LOW);
              //On
              analogWrite(npn2, motorspeed);
              analogWrite(npn3, motorspeed);
              digitalWrite(pnp1, HIGH);
              digitalWrite(pnp4, HIGH);

              irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
              irrightfrontstatus = digitalRead(irrightfrontpin);
              irfrontleftstatus = digitalRead(irfrontleftpin);
              irfrontrightstatus = digitalRead(irfrontrightpin);
              irbackleftstatus = digitalRead(irbackleftpin);
              irbackrightstatus = digitalRead(irbackrightpin);
              irdownleftstatus = digitalRead(irdownleftpin);
              irdownrightstatus = digitalRead(irdownrightpin);

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }
          } else if ((irleftfrontstatus == 1) and (irfrontleftstatus == 1) and ((irrightfrontstatus == 0) or (irfrontrightstatus == 0)) and (mode == 2)) {
            Serial.println("Action 5");
            if (!((irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1)) and (mode == 2)) {
              //Left
              //Off
              digitalWrite(npn2, LOW);
              digitalWrite(npn3, LOW);
              digitalWrite(pnp1, LOW);
              digitalWrite(pnp4, LOW);
              //On
              analogWrite(npn1, motorspeed);
              analogWrite(npn4, motorspeed);
              digitalWrite(pnp2, HIGH);
              digitalWrite(pnp3, HIGH);

              irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
              irrightfrontstatus = digitalRead(irrightfrontpin);
              irfrontleftstatus = digitalRead(irfrontleftpin);
              irfrontrightstatus = digitalRead(irfrontrightpin);
              irbackleftstatus = digitalRead(irbackleftpin);
              irbackrightstatus = digitalRead(irbackrightpin);
              irdownleftstatus = digitalRead(irdownleftpin);
              irdownrightstatus = digitalRead(irdownrightpin);

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }
          } else if ((irleftfrontstatus == 0) and (irfrontleftstatus == 0) and (irfrontrightstatus == 0) and (irrightfrontstatus == 1) and (mode == 2)) {
            Serial.println("Action 6");
            if (!((irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1)) and (mode == 2)) {
              //Right
              //Off
              digitalWrite(npn1, LOW);
              digitalWrite(npn4, LOW);
              digitalWrite(pnp2, LOW);
              digitalWrite(pnp3, LOW);
              //On
              analogWrite(npn2, motorspeed);
              analogWrite(npn3, motorspeed);
              digitalWrite(pnp1, HIGH);
              digitalWrite(pnp4, HIGH);

              irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
              irrightfrontstatus = digitalRead(irrightfrontpin);
              irfrontleftstatus = digitalRead(irfrontleftpin);
              irfrontrightstatus = digitalRead(irfrontrightpin);
              irbackleftstatus = digitalRead(irbackleftpin);
              irbackrightstatus = digitalRead(irbackrightpin);
              irdownleftstatus = digitalRead(irdownleftpin);
              irdownrightstatus = digitalRead(irdownrightpin);

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }
          } else if ((irleftfrontstatus == 1) and (irfrontleftstatus == 0) and (irfrontrightstatus == 0) and (irrightfrontstatus == 0) and (mode == 2)) {
            Serial.println("Action 7");
            if (!((irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1)) and (mode == 2)) {
              //Left
              //Off
              digitalWrite(npn2, LOW);
              digitalWrite(npn3, LOW);
              digitalWrite(pnp1, LOW);
              digitalWrite(pnp4, LOW);
              //On
              analogWrite(npn1, motorspeed);
              analogWrite(npn4, motorspeed);
              digitalWrite(pnp2, HIGH);
              digitalWrite(pnp3, HIGH);

              irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
              irrightfrontstatus = digitalRead(irrightfrontpin);
              irfrontleftstatus = digitalRead(irfrontleftpin);
              irfrontrightstatus = digitalRead(irfrontrightpin);
              irbackleftstatus = digitalRead(irbackleftpin);
              irbackrightstatus = digitalRead(irbackrightpin);
              irdownleftstatus = digitalRead(irdownleftpin);
              irdownrightstatus = digitalRead(irdownrightpin);

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }
          } else if (((irleftfrontstatus == 0) and (irrightfrontstatus == 0)) or ((irfrontleftstatus == 0) and (irfrontrightstatus == 0)) and (mode == 2)) {
            Serial.println("Action 8");
            if (!((irleftfrontstatus == 1) and (irrightfrontstatus == 1) and (irfrontleftstatus == 1) and (irfrontrightstatus == 1)) and (irbackleftstatus == 1) and (irbackrightstatus == 1) and (mode == 2)) {
              //Backward
              //Off
              digitalWrite(npn2, LOW);
              digitalWrite(npn4, LOW);
              digitalWrite(pnp1, LOW);
              digitalWrite(pnp3, LOW);
              //On
              analogWrite(npn1, motorspeed);
              analogWrite(npn3, motorspeed);
              digitalWrite(pnp2, HIGH);
              digitalWrite(pnp4, HIGH);

              irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
              irrightfrontstatus = digitalRead(irrightfrontpin);
              irfrontleftstatus = digitalRead(irfrontleftpin);
              irfrontrightstatus = digitalRead(irfrontrightpin);
              irbackleftstatus = digitalRead(irbackleftpin);
              irbackrightstatus = digitalRead(irbackrightpin);
              irdownleftstatus = digitalRead(irdownleftpin);
              irdownrightstatus = digitalRead(irdownrightpin);

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }

              if (((leftfrontdistance + frontleftsensordistance) / 2) > ((rightfrontdistance + frontrightsensordistance) / 2)) {
                Serial.println("Action 9");
                if (!((leftfrontdistance > (20 + additionalsensitivity)) and (frontleftsensordistance > (20 + additionalsensitivity)) and (frontrightsensordistance > (20 + additionalsensitivity)) and (rightfrontdistance > (20 + additionalsensitivity)) and (frontdistance > (25 + additionalsensitivity))) and (mode == 2)) {
                  //Left
                  //Off
                  digitalWrite(npn2, LOW);
                  digitalWrite(npn3, LOW);
                  digitalWrite(pnp1, LOW);
                  digitalWrite(pnp4, LOW);
                  //On
                  analogWrite(npn1, motorspeed);
                  analogWrite(npn4, motorspeed);
                  digitalWrite(pnp2, HIGH);
                  digitalWrite(pnp3, HIGH);

                  digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
                  delayMicroseconds(5);
                  digitalWrite(leftfronttrigpin, HIGH);
                  delayMicroseconds(5);
                  digitalWrite(leftfronttrigpin, LOW);
                  duration = pulseIn(leftfrontechopin, HIGH, 20000);
                  if ((duration * 0.034 / 2) < 300) {
                    leftfrontdistance = duration * 0.034 / 2;
                  }
                  digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
                  delayMicroseconds(5);
                  digitalWrite(frontleftsensortrigpin, HIGH);
                  delayMicroseconds(5);
                  digitalWrite(frontleftsensortrigpin, LOW);
                  duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
                  if ((duration * 0.034 / 2) < 300) {
                    frontleftsensordistance = duration * 0.034 / 2;
                  }
                  digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
                  delayMicroseconds(5);
                  digitalWrite(frontrightsensortrigpin, HIGH);
                  delayMicroseconds(5);
                  digitalWrite(frontrightsensortrigpin, LOW);
                  duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
                  if ((duration * 0.034 / 2) < 300) {
                    frontrightsensordistance = duration * 0.034 / 2;
                  }
                  digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
                  delayMicroseconds(5);
                  digitalWrite(rightfronttrigpin, HIGH);
                  delayMicroseconds(5);
                  digitalWrite(rightfronttrigpin, LOW);
                  duration = pulseIn(rightfrontechopin, HIGH, 20000);
                  if ((duration * 0.034 / 2) < 300) {
                    rightfrontdistance = duration * 0.034 / 2;
                  }
                  digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
                  delayMicroseconds(5);
                  digitalWrite(fronttrigpin, HIGH);
                  delayMicroseconds(5);
                  digitalWrite(fronttrigpin, LOW);
                  duration = pulseIn(frontechopin, HIGH, 20000);
                  if ((duration * 0.034 / 2) < 300) {
                    frontdistance = duration * 0.034 / 2;
                  }
                  if (leftfrontdistance == 0) leftfrontdistance = 50000;
                  if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
                  if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
                  if (rightfrontdistance == 0) rightfrontdistance = 50000;
                  if (frontdistance == 0) frontdistance = 50000;

                  throttleval = pulseIn(throttlepin, HIGH); //Check mode
                  steeringval = pulseIn(steeringpin, HIGH);
                  modeval = pulseIn(modepin, HIGH);
                  if (modeval < 1250) {
                    mode = 1;
                    newmodechange = 1;
                  }
                  else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
                  else if (modeval > 1750) {
                    mode = 3;
                    newmodechange = 1;
                  }
                }
              }
            }
          } else {
            if (((leftfrontdistance + frontleftsensordistance) / 2) > ((rightfrontdistance + frontrightsensordistance) / 2)) {
              Serial.println("Action 10");
              if (!((leftfrontdistance > (20 + additionalsensitivity)) and (frontleftsensordistance > (20 + additionalsensitivity)) and (frontrightsensordistance > (20 + additionalsensitivity)) and (rightfrontdistance > (20 + additionalsensitivity)) and (frontdistance > (25 + additionalsensitivity))) and (mode == 2)) {
                //Left
                //Off
                digitalWrite(npn2, LOW);
                digitalWrite(npn3, LOW);
                digitalWrite(pnp1, LOW);
                digitalWrite(pnp4, LOW);
                //On
                analogWrite(npn1, motorspeed);
                analogWrite(npn4, motorspeed);
                digitalWrite(pnp2, HIGH);
                digitalWrite(pnp3, HIGH);

                digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
                delayMicroseconds(5);
                digitalWrite(leftfronttrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(leftfronttrigpin, LOW);
                duration = pulseIn(leftfrontechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  leftfrontdistance = duration * 0.034 / 2;
                }
                digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
                delayMicroseconds(5);
                digitalWrite(frontleftsensortrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(frontleftsensortrigpin, LOW);
                duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  frontleftsensordistance = duration * 0.034 / 2;
                }
                digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
                delayMicroseconds(5);
                digitalWrite(frontrightsensortrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(frontrightsensortrigpin, LOW);
                duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  frontrightsensordistance = duration * 0.034 / 2;
                }
                digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
                delayMicroseconds(5);
                digitalWrite(rightfronttrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(rightfronttrigpin, LOW);
                duration = pulseIn(rightfrontechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  rightfrontdistance = duration * 0.034 / 2;
                }
                digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
                delayMicroseconds(5);
                digitalWrite(fronttrigpin, HIGH);
                delayMicroseconds(5);
                digitalWrite(fronttrigpin, LOW);
                duration = pulseIn(frontechopin, HIGH, 20000);
                if ((duration * 0.034 / 2) < 300) {
                  frontdistance = duration * 0.034 / 2;
                }
                if (leftfrontdistance == 0) leftfrontdistance = 50000;
                if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
                if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
                if (rightfrontdistance == 0) rightfrontdistance = 50000;
                if (frontdistance == 0) frontdistance = 50000;

                throttleval = pulseIn(throttlepin, HIGH); //Check mode
                steeringval = pulseIn(steeringpin, HIGH);
                modeval = pulseIn(modepin, HIGH);
                if (modeval < 1250) {
                  mode = 1;
                  newmodechange = 1;
                }
                else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
                else if (modeval > 1750) {
                  mode = 3;
                  newmodechange = 1;
                }
              }
            } else if (!((leftfrontdistance > (20 + additionalsensitivity)) and (frontleftsensordistance > (20 + additionalsensitivity)) and (frontrightsensordistance > (20 + additionalsensitivity)) and (rightfrontdistance > (20 + additionalsensitivity)) and (frontdistance > (25 + additionalsensitivity))) and (mode == 2)) {
              //Right
              //Off
              digitalWrite(npn1, LOW);
              digitalWrite(npn4, LOW);
              digitalWrite(pnp2, LOW);
              digitalWrite(pnp3, LOW);
              //On
              analogWrite(npn2, motorspeed);
              analogWrite(npn3, motorspeed);
              digitalWrite(pnp1, HIGH);
              digitalWrite(pnp4, HIGH);

              digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
              delayMicroseconds(5);
              digitalWrite(leftfronttrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(leftfronttrigpin, LOW);
              duration = pulseIn(leftfrontechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                leftfrontdistance = duration * 0.034 / 2;
              }
              digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
              delayMicroseconds(5);
              digitalWrite(frontleftsensortrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(frontleftsensortrigpin, LOW);
              duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                frontleftsensordistance = duration * 0.034 / 2;
              }
              digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
              delayMicroseconds(5);
              digitalWrite(frontrightsensortrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(frontrightsensortrigpin, LOW);
              duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                frontrightsensordistance = duration * 0.034 / 2;
              }
              digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
              delayMicroseconds(5);
              digitalWrite(rightfronttrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(rightfronttrigpin, LOW);
              duration = pulseIn(rightfrontechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                rightfrontdistance = duration * 0.034 / 2;
              }
              digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
              delayMicroseconds(5);
              digitalWrite(fronttrigpin, HIGH);
              delayMicroseconds(5);
              digitalWrite(fronttrigpin, LOW);
              duration = pulseIn(frontechopin, HIGH, 20000);
              if ((duration * 0.034 / 2) < 300) {
                frontdistance = duration * 0.034 / 2;
              }
              if (leftfrontdistance == 0) leftfrontdistance = 50000;
              if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
              if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
              if (rightfrontdistance == 0) rightfrontdistance = 50000;
              if (frontdistance == 0) frontdistance = 50000;

              throttleval = pulseIn(throttlepin, HIGH); //Check mode
              steeringval = pulseIn(steeringpin, HIGH);
              modeval = pulseIn(modepin, HIGH);
              if (modeval < 1250) {
                mode = 1;
                newmodechange = 1;
              }
              else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
              else if (modeval > 1750) {
                mode = 3;
                newmodechange = 1;
              }
            }

          }
        } else {
          Serial.println("Action 11");
          //Stop
          digitalWrite(npn1, LOW);
          digitalWrite(npn2, LOW);
          digitalWrite(npn3, LOW);
          digitalWrite(npn4, LOW);
          digitalWrite(pnp1, LOW);
          digitalWrite(pnp2, LOW);
          digitalWrite(pnp3, LOW);
          digitalWrite(pnp4, LOW);

          irleftfrontstatus = digitalRead(irleftfrontpin); //Check IR sensor
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          throttleval = pulseIn(throttlepin, HIGH); //Check mode
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);
          if (modeval < 1250) {
            mode = 1;
            newmodechange = 1;
          }
          else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
          else if (modeval > 1750) {
            mode = 3;
            newmodechange = 1;
          }
        }
      } else if (throttlemode == 2) {
        throttleval = pulseIn(throttlepin, HIGH);
        steeringval = pulseIn(steeringpin, HIGH);
        modeval = pulseIn(modepin, HIGH);
        motorspeed = map(steeringval, 1000, 2000, 0, 255);
        if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
        else if (throttleval < 1400) throttlemode = 1;
        else if (throttleval > 1600) throttlemode = 2;
      }
    }
  } else {
    digitalWrite(13, LOW);
    delay(500);
    Serial.print("Low battery with the voltage of: ");
    Serial.print(batteryvoltage);
    Serial.println(" Enter anything to enter diagnostics mode.");
    if (Serial.available() > 0) {
      while (batteryvoltage < 6.60) {
        batteryvoltage = ((analogRead(voltagepin) * (5.00 / 1023.00)) * 2.00) - 0.57;
        if (batteryvoltage < 0) batteryvoltage = 0;
        additionalsensitivity = map(motorspeed, 0, 255, 0, 10) - map(batteryvoltage, 6.60, 8.40, 10, 0);
        if (additionalsensitivity < -15) additionalsensitivity = -15;
        if (steeringmode == 0) { //Print remote control parameters
          digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, LOW);
          duration = pulseIn(leftfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            leftfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, LOW);
          duration = pulseIn(rightfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            rightfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, LOW);
          duration = pulseIn(frontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontdistance = duration * 0.034 / 2;
          }


          irleftfrontstatus = digitalRead(irleftfrontpin);
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          if (leftfrontdistance == 0) leftfrontdistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (rightfrontdistance == 0) rightfrontdistance = 50000;
          if (frontdistance == 0) frontdistance = 50000;

          throttleval = pulseIn(throttlepin, HIGH);
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          Serial.print("Speed: ");
          Serial.print(motorspeed);
          Serial.print(" | Voltage: ");
          Serial.print(batteryvoltage);
          Serial.print("V | Sensitivity: ");
          Serial.print(additionalsensitivity);
          Serial.print(" | Throttle: ");
          Serial.print(throttleval);
          Serial.print(" | Steering: ");
          Serial.print(steeringval);
          Serial.print(" | Mode: ");
          Serial.println(modeval);


          if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
          else if (throttleval < 1400) throttlemode = 1;
          else if (throttleval > 1600) throttlemode = 2;

          if ((steeringval > 1400) and (steeringval < 1600)) steeringmode = 0; //Check steering mode
          else if (steeringval < 1400) steeringmode = 1;
          else if (steeringval > 1600) steeringmode = 2;
        }

        if (steeringmode == 1) { //Print ultrasonic sensors parameters
          digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, LOW);
          duration = pulseIn(leftfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            leftfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, LOW);
          duration = pulseIn(rightfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            rightfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, LOW);
          duration = pulseIn(frontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontdistance = duration * 0.034 / 2;
          }


          irleftfrontstatus = digitalRead(irleftfrontpin);
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          if (leftfrontdistance == 0) leftfrontdistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (rightfrontdistance == 0) rightfrontdistance = 50000;
          if (frontdistance == 0) frontdistance = 50000;

          throttleval = pulseIn(throttlepin, HIGH);
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          Serial.print("Left Front: ");
          Serial.print(leftfrontdistance);
          Serial.print(" | Front Left: ");
          Serial.print(frontleftsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Right Front: ");
          Serial.print(rightfrontdistance);
          Serial.print(" | Front: ");
          Serial.println(frontdistance);

          if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
          else if (throttleval < 1400) throttlemode = 1;
          else if (throttleval > 1600) throttlemode = 2;

          if ((steeringval > 1400) and (steeringval < 1600)) steeringmode = 0; //Check steering mode
          else if (steeringval < 1400) steeringmode = 1;
          else if (steeringval > 1600) steeringmode = 2;
        }

        if (steeringmode == 2) { //Print IR sensor parameters
          digitalWrite(leftfronttrigpin, LOW); //Check distance for left front sensor
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(leftfronttrigpin, LOW);
          duration = pulseIn(leftfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            leftfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(rightfronttrigpin, LOW); //Check distance for right front sensor
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(rightfronttrigpin, LOW);
          duration = pulseIn(rightfrontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            rightfrontdistance = duration * 0.034 / 2;
          }
          digitalWrite(fronttrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, HIGH);
          delayMicroseconds(5);
          digitalWrite(fronttrigpin, LOW);
          duration = pulseIn(frontechopin, HIGH, 20000);
          if ((duration * 0.034 / 2) < 300) {
            frontdistance = duration * 0.034 / 2;
          }


          irleftfrontstatus = digitalRead(irleftfrontpin);
          irrightfrontstatus = digitalRead(irrightfrontpin);
          irfrontleftstatus = digitalRead(irfrontleftpin);
          irfrontrightstatus = digitalRead(irfrontrightpin);
          irbackleftstatus = digitalRead(irbackleftpin);
          irbackrightstatus = digitalRead(irbackrightpin);
          irdownleftstatus = digitalRead(irdownleftpin);
          irdownrightstatus = digitalRead(irdownrightpin);

          if (leftfrontdistance == 0) leftfrontdistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (rightfrontdistance == 0) rightfrontdistance = 50000;
          if (frontdistance == 0) frontdistance = 50000;

          throttleval = pulseIn(throttlepin, HIGH);
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          Serial.print("Left Front IR: ");
          Serial.print(irleftfrontstatus);
          Serial.print(" | Front Left IR: ");
          Serial.print(irfrontleftstatus);
          Serial.print(" | Front Right IR: ");
          Serial.print(irfrontrightstatus);
          Serial.print(" | Right Front IR: ");
          Serial.print(irrightfrontstatus);
          Serial.print(" | Back Left IR: ");
          Serial.print(irbackleftstatus);
          Serial.print(" | Back Right IR: ");
          Serial.print(irbackrightstatus);
          Serial.print(" | Down Left IR: ");
          Serial.print(irdownleftstatus);
          Serial.print(" | Down Right IR: ");
          Serial.println(irdownrightstatus);

          if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
          else if (throttleval < 1400) throttlemode = 1;
          else if (throttleval > 1600) throttlemode = 2;

          if ((steeringval > 1400) and (steeringval < 1600)) steeringmode = 0; //Check steering mode
          else if (steeringval < 1400) steeringmode = 1;
          else if (steeringval > 1600) steeringmode = 2;
        }
      }

    }

  }
}
