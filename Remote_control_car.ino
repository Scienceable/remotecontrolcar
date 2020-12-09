#include <Servo.h>

Servo frontsensor;

const int pnp1 = 2;
const int pnp2 = 3;
const int pnp3 = 4;
const int pnp4 = 5;
const int npn1 = 6;
const int npn2 = 7;
const int npn3 = 8;
const int npn4 = 9;

const int throttlepin = 10;
const int steeringpin = 11;
const int modepin = 12;

const int frontsensorservopin = 50;

const int frontsensorechopin = 22;
const int frontsensortrigpin = 23;
const int frontrightsensorechopin = 42;
const int frontrightsensortrigpin = 43;
const int frontleftsensorechopin = 46;
const int frontleftsensortrigpin = 47;

int rotation;
int mode = 1;

int throttleval;
int steeringval;
int modeval;

unsigned int frontsensordistance = 50000;
unsigned int frontrightsensordistance = 50000;
unsigned int frontleftsensordistance = 50000;
unsigned int duration;

int frontsensorservoangle = 90;
int frontsensorservorotation = 0;

int throttlemode = 0;





void setup() {
  Serial.begin(9600); // Starts the serial communication

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

  pinMode(frontsensortrigpin, OUTPUT);
  pinMode(frontsensorechopin, INPUT_PULLUP);
  pinMode(frontrightsensortrigpin, OUTPUT);
  pinMode(frontrightsensorechopin, INPUT_PULLUP);
  pinMode(frontleftsensortrigpin, OUTPUT);
  pinMode(frontleftsensorechopin, INPUT_PULLUP);
  digitalWrite(frontsensortrigpin, LOW);
  digitalWrite(frontrightsensortrigpin, LOW);
  digitalWrite(frontleftsensortrigpin, LOW);

  frontsensor.attach(frontsensorservopin);
  frontsensor.write(frontsensorservoangle);
}

void loop() {


  throttleval = pulseIn(throttlepin, HIGH);
  steeringval = pulseIn(steeringpin, HIGH);
  modeval = pulseIn(modepin, HIGH);

  if (modeval < 1250) mode = 1;
  else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
  else if (modeval > 1750) mode = 3;
  if (mode == 1) {
    Serial.print("Throttle:");
    Serial.print(pulseIn(throttlepin, HIGH));
    Serial.print(" | Steering:");
    Serial.print(pulseIn(steeringpin, HIGH));
    Serial.print(" | Mode:");
    Serial.println(pulseIn(modepin, HIGH));

    frontsensor.write(90);

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
      digitalWrite(npn2, HIGH);
      digitalWrite(npn4, HIGH);
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
      digitalWrite(npn1, HIGH);
      digitalWrite(npn3, HIGH);
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
      digitalWrite(npn2, HIGH);
      digitalWrite(npn3, HIGH);
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
      digitalWrite(npn1, HIGH);
      digitalWrite(npn4, HIGH);
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
      digitalWrite(npn2, HIGH);
      analogWrite(npn4, 140);
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
      analogWrite(npn2, 140);
      digitalWrite(npn4, HIGH);
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
      digitalWrite(npn1, HIGH);
      analogWrite(npn3, 140);
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
      analogWrite(npn1, 100);
      digitalWrite(npn3, HIGH);
      digitalWrite(pnp2, HIGH);
      digitalWrite(pnp4, HIGH);
    }
  }

  if (mode == 2) {
    if (throttlemode == 0) {
      frontsensorservoangle = 90;
      frontsensor.write(frontsensorservoangle);

      digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
      delayMicroseconds(5);
      digitalWrite(frontsensortrigpin, HIGH);
      delayMicroseconds(20);
      digitalWrite(frontsensortrigpin, LOW);
      duration = pulseIn(frontsensorechopin, HIGH, 10000);
      if ((duration * 0.034 / 2) < 200) {
        frontsensordistance = duration * 0.034 / 2;
      }
      digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
      delayMicroseconds(5);
      digitalWrite(frontrightsensortrigpin, HIGH);
      delayMicroseconds(20);
      digitalWrite(frontrightsensortrigpin, LOW);
      duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
      if ((duration * 0.034 / 2) < 200) {
        frontrightsensordistance = duration * 0.034 / 2;
      }
      digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
      delayMicroseconds(5);
      digitalWrite(frontleftsensortrigpin, HIGH);
      delayMicroseconds(20);
      digitalWrite(frontleftsensortrigpin, LOW);
      duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
      if ((duration * 0.034 / 2) < 200) {
        frontleftsensordistance = duration * 0.034 / 2;
      }
      if (frontsensordistance == 0) frontsensordistance = 50000;
      if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
      if (frontleftsensordistance == 0) frontleftsensordistance = 50000;

      throttleval = pulseIn(throttlepin, HIGH);
      steeringval = pulseIn(steeringpin, HIGH);
      modeval = pulseIn(modepin, HIGH);

      Serial.print("Throttle:");
      Serial.print(throttleval);
      Serial.print(" | Steering:");
      Serial.print(steeringval);
      Serial.print(" | Mode:");
      Serial.print(modeval);
      Serial.print(" | Front: ");
      Serial.print(frontsensordistance);
      Serial.print(" | Front Right: ");
      Serial.print(frontrightsensordistance);
      Serial.print(" | Front Left: ");
      Serial.println(frontleftsensordistance);

      if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 0; //Check throttle mode
      else if (throttleval < 1400) throttlemode = 1;
      else if (throttleval > 1600) throttlemode = 2;

    }

    if (throttlemode == 1) {

      digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
      delayMicroseconds(5);
      digitalWrite(frontrightsensortrigpin, HIGH);
      delayMicroseconds(20);
      digitalWrite(frontrightsensortrigpin, LOW);
      duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
      if ((duration * 0.034 / 2) < 200) {
        frontrightsensordistance = duration * 0.034 / 2;
      }
      digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
      delayMicroseconds(5);
      digitalWrite(frontleftsensortrigpin, HIGH);
      delayMicroseconds(20);
      digitalWrite(frontleftsensortrigpin, LOW);
      duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
      if ((duration * 0.034 / 2) < 200) {
        frontleftsensordistance = duration * 0.034 / 2;
      }
      digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
      delayMicroseconds(5);
      digitalWrite(frontsensortrigpin, HIGH);
      delayMicroseconds(20);
      digitalWrite(frontsensortrigpin, LOW);
      duration = pulseIn(frontsensorechopin, HIGH, 10000);
      if ((duration * 0.034 / 2) < 200) {
        frontsensordistance = duration * 0.034 / 2;
      }
      if (frontsensordistance == 0) frontsensordistance = 50000;
      if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
      if (frontleftsensordistance == 0) frontleftsensordistance = 50000;

      if ((frontsensordistance > 60) and (frontrightsensordistance > 30) and (frontleftsensordistance > 30)) {
        while ((frontsensordistance > 20) and (frontrightsensordistance > 30) and (frontleftsensordistance > 30) and (mode == 2)) {
          //Forward
          //Off
          digitalWrite(npn1, LOW);
          digitalWrite(npn3, LOW);
          digitalWrite(pnp2, LOW);
          digitalWrite(pnp4, LOW);
          //On
          digitalWrite(npn2, HIGH);
          digitalWrite(npn4, HIGH);
          digitalWrite(pnp1, HIGH);
          digitalWrite(pnp3, HIGH);

          frontsensor.write(frontsensorservoangle);
          delay(0);
          if ((frontsensorservoangle == 90) and (rotation == 0)) {
            frontsensorservoangle = 105;
          } else if ((frontsensorservoangle == 90) and (rotation == 1)) {
            frontsensorservoangle = 75;
          } else if ((frontsensorservoangle == 75) and (rotation == 0)) {
            frontsensorservoangle = 90;
          } else if ((frontsensorservoangle == 75) and (rotation == 1)) {
            frontsensorservoangle = 60;
          } else if (frontsensorservoangle == 60) {
            frontsensorservoangle = 75;
            rotation = 0;
          } else if ((frontsensorservoangle == 105) and (rotation == 0)) {
            frontsensorservoangle = 120;
          } else if ((frontsensorservoangle == 105) and (rotation == 1)) {
            frontsensorservoangle = 90;
          } else if (frontsensorservoangle == 120) {
            frontsensorservoangle = 105;
            rotation = 1;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(frontsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontsensortrigpin, LOW);
          duration = pulseIn(frontsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontsensordistance = duration * 0.034 / 2;
          }
          if (frontsensordistance == 0) frontsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;
          Serial.print("Front: ");
          Serial.print(frontsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Front Left : ");
          Serial.println(frontleftsensordistance);


          throttleval = pulseIn(throttlepin, HIGH); //Check mode
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          if (modeval < 1250) mode = 1;
          else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
          else if (modeval > 1750) mode = 3;
        }
      } else if ((frontleftsensordistance < 30) and (frontleftsensordistance < frontrightsensordistance)) {
        while ((frontsensordistance < 60) or (frontrightsensordistance < 30) or (frontleftsensordistance < 30) and (frontleftsensordistance < frontrightsensordistance) and (mode == 2)) {
          //Right
          //Off
          digitalWrite(npn1, LOW);
          digitalWrite(npn4, LOW);
          digitalWrite(pnp2, LOW);
          digitalWrite(pnp3, LOW);
          //On
          digitalWrite(npn2, HIGH);
          digitalWrite(npn3, HIGH);
          digitalWrite(pnp1, HIGH);
          digitalWrite(pnp4, HIGH);

          digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(frontsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontsensortrigpin, LOW);
          duration = pulseIn(frontsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          if (frontsensordistance == 0) frontsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;

          Serial.print("Front: ");
          Serial.print(frontsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Front Left : ");
          Serial.println(frontleftsensordistance);
        }
      } else if ((frontrightsensordistance < 30) and (frontrightsensordistance < frontleftsensordistance)) {
        while ((frontsensordistance < 60) or (frontrightsensordistance < 30) or (frontleftsensordistance < 30) and (frontrightsensordistance < frontleftsensordistance) and (mode == 2)) {
          //Left
          //Off
          digitalWrite(npn2, LOW);
          digitalWrite(npn3, LOW);
          digitalWrite(pnp1, LOW);
          digitalWrite(pnp4, LOW);
          //On
          digitalWrite(npn1, HIGH);
          digitalWrite(npn4, HIGH);
          digitalWrite(pnp2, HIGH);
          digitalWrite(pnp3, HIGH);

          digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(frontsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontsensortrigpin, LOW);
          duration = pulseIn(frontsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          if (frontsensordistance == 0) frontsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;

          Serial.print("Front: ");
          Serial.print(frontsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Front Left : ");
          Serial.println(frontleftsensordistance);


          throttleval = pulseIn(throttlepin, HIGH); //Check mode
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

          if (modeval < 1250) mode = 1;
          else if ((modeval > 1250) and (modeval < 1750)) mode = 2;
          else if (modeval > 1750) mode = 3;
        }

      } else if (frontleftsensordistance < frontrightsensordistance) {
        while ((frontsensordistance < 60) or (frontrightsensordistance < 30) or (frontleftsensordistance < 30) and (frontleftsensordistance < frontrightsensordistance) and (mode == 2)) {
          //Right
          //Off
          digitalWrite(npn1, LOW);
          digitalWrite(npn4, LOW);
          digitalWrite(pnp2, LOW);
          digitalWrite(pnp3, LOW);
          //On
          digitalWrite(npn2, HIGH);
          digitalWrite(npn3, HIGH);
          digitalWrite(pnp1, HIGH);
          digitalWrite(pnp4, HIGH);

          digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(frontsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontsensortrigpin, LOW);
          duration = pulseIn(frontsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          if (frontsensordistance == 0) frontsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;

          Serial.print("Front: ");
          Serial.print(frontsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Front Left : ");
          Serial.println(frontleftsensordistance);

          throttleval = pulseIn(throttlepin, HIGH); //Check mode
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);
        }
      } else if (frontrightsensordistance < frontleftsensordistance) {
        while ((frontsensordistance < 60) or (frontrightsensordistance < 30) or (frontleftsensordistance < 30) and (frontrightsensordistance < frontleftsensordistance) and (mode == 2)) {
          //Left
          //Off
          digitalWrite(npn2, LOW);
          digitalWrite(npn3, LOW);
          digitalWrite(pnp1, LOW);
          digitalWrite(pnp4, LOW);
          //On
          digitalWrite(npn1, HIGH);
          digitalWrite(npn4, HIGH);
          digitalWrite(pnp2, HIGH);
          digitalWrite(pnp3, HIGH);

          digitalWrite(frontsensortrigpin, LOW); //Check distance for front sensor
          delayMicroseconds(5);
          digitalWrite(frontsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontsensortrigpin, LOW);
          duration = pulseIn(frontsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontrightsensortrigpin, LOW); //Check distance for front right sensor
          delayMicroseconds(5);
          digitalWrite(frontrightsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontrightsensortrigpin, LOW);
          duration = pulseIn(frontrightsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontrightsensordistance = duration * 0.034 / 2;
          }
          digitalWrite(frontleftsensortrigpin, LOW); //Check distance for front left sensor
          delayMicroseconds(5);
          digitalWrite(frontleftsensortrigpin, HIGH);
          delayMicroseconds(20);
          digitalWrite(frontleftsensortrigpin, LOW);
          duration = pulseIn(frontleftsensorechopin, HIGH, 10000);
          if ((duration * 0.034 / 2) < 200) {
            frontleftsensordistance = duration * 0.034 / 2;
          }
          if (frontsensordistance == 0) frontsensordistance = 50000;
          if (frontrightsensordistance == 0) frontrightsensordistance = 50000;
          if (frontleftsensordistance == 0) frontleftsensordistance = 50000;

          Serial.print("Front: ");
          Serial.print(frontsensordistance);
          Serial.print(" | Front Right: ");
          Serial.print(frontrightsensordistance);
          Serial.print(" | Front Left : ");
          Serial.println(frontleftsensordistance);


          throttleval = pulseIn(throttlepin, HIGH); //Check mode
          steeringval = pulseIn(steeringpin, HIGH);
          modeval = pulseIn(modepin, HIGH);

        }

      }
      throttleval = pulseIn(throttlepin, HIGH); //Check mode
      steeringval = pulseIn(steeringpin, HIGH);
      modeval = pulseIn(modepin, HIGH);
      if ((throttleval > 1400) and (throttleval < 1600)) throttlemode = 1; //Check throttle mode
      else if (throttleval < 1400) throttlemode = 1;
      else if (throttleval > 1600) throttlemode = 0;
    }
  }
}
