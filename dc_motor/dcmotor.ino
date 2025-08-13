/*
Pins:
Vcc  | Vcc   (compatible with 3.3v or 5v power)
PWM1 | PWM2  (PWM input for speed control of motors 1 and 2) 
INA1 | INA2  (motors 1 and 2 control logic input A)
INB1 | INB2  (motors 1 and 2 control logic input B)
GND  | GND

Run logic:
INAx low, INBx low  -> motor stopped
INAx low, INBx high -> motor forward
INAx high, INBx low -> motor reverse
*/

int PWM1 = 2;
int INA1 = 4;
int INB1 = 6;
int PWM2 = 12;
int INA2 = 10;
int INB2 = 8;

void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(INB2, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Motor Control Program");
}

// Helper function: wait for and read user input (trimmed)
String readInput() {
  while (Serial.available() == 0) { }
  String s = Serial.readStringUntil('\n');
  s.trim();
  return s;
}

// Helper function to stop a given motor (both direction pins low, PWM off)
void stopMotor(int motor) {
  if (motor == 1) {
    digitalWrite(INA1, LOW);
    digitalWrite(INB1, LOW);
    analogWrite(PWM1, 0);
  } else if (motor == 2) {
    digitalWrite(INA2, LOW);
    digitalWrite(INB2, LOW);
    analogWrite(PWM2, 0);
  }
}

void runMotor(int motor, int speed, bool forward, unsigned long runTimeSec, bool continuous) {
  int PWM, INA, INB;
  if (motor == 1) {
    PWM = PWM1;
    INA = INA1;
    INB = INB1;
  } else { // motor 2
    PWM = PWM2;
    INA = INA2;
    INB = INB2;
  }
  // Set motor direction
  if (forward) {
    digitalWrite(INA, LOW);
    digitalWrite(INB, HIGH);
  } else {
    digitalWrite(INA, HIGH);
    digitalWrite(INB, LOW);
  }
  // Map speed (1-100) to PWM value (32-255)
  int pwmValue = map(speed, 1, 100, 32, 255);
  
  if (continuous) {
    Serial.println("Running continuous mode. Type 'exit' to stop.");
    analogWrite(PWM, pwmValue);
    while (true) {
      if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("exit")) {
          Serial.println("Exiting continuous mode.");
          break;
        }
      }
      delay(10);
    }
  } else {
    unsigned long startTime = millis();
    while (millis() - startTime < runTimeSec * 1000UL) {
      analogWrite(PWM, pwmValue);
    }
  }
  
  // Stop the motor after operation
  stopMotor(motor);
}

void runBothMotors(int speed, bool forward, unsigned long runTimeSec, bool continuous) {
  // Set direction for both motors
  if (forward) {
    digitalWrite(INA1, LOW);
    digitalWrite(INB1, HIGH);
    digitalWrite(INA2, LOW);
    digitalWrite(INB2, HIGH);
  } else {
    digitalWrite(INA1, HIGH);
    digitalWrite(INB1, LOW);
    digitalWrite(INA2, HIGH);
    digitalWrite(INB2, LOW);
  }
  
  int pwmValue = map(speed, 1, 100, 32, 255);
  
  if (continuous) {
    Serial.println("Running continuous mode for both motors. Type 'exit' to stop.");
    analogWrite(PWM1, pwmValue);
    analogWrite(PWM2, pwmValue);
    while (true) {
      if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("exit")) {
          Serial.println("Exiting continuous mode.");
          break;
        }
      }
      delay(10);
    }
  } else {
    unsigned long startTime = millis();
    while (millis() - startTime < runTimeSec * 1000UL) {
      analogWrite(PWM1, pwmValue);
      analogWrite(PWM2, pwmValue);
    }
  }
  
  // Stop both motors
  stopMotor(1);
  stopMotor(2);
}

void loop() {
  int motor = 0;

  Serial.println("Select motor: 1, 2, or 3 for both:");

  // Keep prompting every 2 seconds if there's no input
  while (true) {
    if (Serial.available() > 0) {
      motor = readInput().toInt();
      if (motor >= 1 && motor <= 3) {
        break; // Valid input
      } else {
        Serial.println("Invalid motor selection. Please enter 1, 2, or 3.");
      }
    } else {
      delay(2000); // Wait 2s before repeating the prompt
      Serial.println("Select motor: 1, 2, or 3 for both:");
    }
  }

  // 2. Select direction
  Serial.println("Select direction (0 for forward, 1 for reverse):");
  int dirInput = readInput().toInt();
  bool forward = (dirInput == 0);

  // 3. Select run mode
  Serial.println("Select run mode: T for timed run, C for continuous:");
  String mode = readInput();
  mode.toUpperCase();

  bool continuous = false;
  unsigned long runTimeSec = 0;

  if (mode == "T") {
    Serial.println("Enter run time in seconds:");
    runTimeSec = readInput().toInt();
  } else if (mode == "C") {
    continuous = true;
  } else {
    Serial.println("Invalid run mode. Restarting cycle.\n");
    delay(500);
    return;
  }

  // 4. Enter speed (1-100)
  Serial.println("Enter speed (1-100):");
  int speed = readInput().toInt();
  if (speed < 1 || speed > 100) {
    Serial.println("Invalid speed. Restarting cycle.\n");
    delay(500);
    return;
  }

  // Run the selected motor(s)
  if (motor == 3) {
    runBothMotors(speed, forward, runTimeSec, continuous);
  } else {
    runMotor(motor, speed, forward, runTimeSec, continuous);
  }

  Serial.println("Operation completed. Starting new cycle.\n");
  delay(500);
}

