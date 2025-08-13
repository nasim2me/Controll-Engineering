#define pul1 3
#define dir1 5
#define pul2 7
#define dir2 9

const int stepsPerRevolution =800;  // Adjust as needed for your motor

String readString() {
  while (!Serial.available()) { }
  String s = Serial.readStringUntil('\n');
  s.trim();
  return s;
}

void setup() {
  pinMode(pul1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pul2, OUTPUT);
  pinMode(dir2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Stepper Motor Control");
}

void loop() {
  // 1. Select Motor
  Serial.println("Select Motor (1 or 2):");
  int motor = readString().toInt();
  if (motor != 1 && motor != 2) {
    Serial.println("Invalid motor selection. Try again.");
    return;
  }
  
  // 2. Select Direction
  Serial.println("Enter direction (0 for forward direction, 1 for reverse):");
  int direction = readString().toInt();
  
  // Determine which pins to use based on motor selection
  int pulPin, dirPin;
  if (motor == 1) {
    pulPin = pul1;
    dirPin = dir1;
  } else { // motor == 2
    pulPin = pul2;
    dirPin = dir2;
  }
  
  // Set motor direction
  if (direction == 0) {
    digitalWrite(dirPin, LOW);
  } else {
    digitalWrite(dirPin, HIGH);
  }
  
  // 3. Select mode: finite or continuous
  Serial.println("Select mode: F for finite turns, C for continuous:");
  String mode = readString();
  mode.toUpperCase();  // Ensure mode is uppercase for comparison
  
  long totalSteps = 0;
  if (mode == "F") {
    Serial.println("Enter number of turns (integer):");
    int turns = readString().toInt();
    totalSteps = (long)turns * stepsPerRevolution;
  } else if (mode != "C") {
    Serial.println("Invalid mode selection. Try again.");
    return;
  }
  
  // 4. Enter speed value (1-100)
  Serial.println("Enter speed (1-100):");
  int speed = readString().toInt();
  if (speed < 1 || speed > 100) {
    Serial.println("Invalid speed. Try again.");
    return;
  }
  
  // Map speed (1-100) to delay time in microseconds.
  // A lower delay means higher speed.
  unsigned int pulseDuration = map(speed, 1, 100, 800, 200);
  
  Serial.println("Starting motor...");
  
  // 5. Run the motor according to the selected mode
  if (mode == "F" || mode == "f") {
    // Finite mode: run for the calculated number of steps.
    for (long i = 0; i < totalSteps; i++) {
      digitalWrite(pulPin, HIGH);
      delayMicroseconds(pulseDuration);
      digitalWrite(pulPin, LOW);
      delayMicroseconds(pulseDuration);
    }
    Serial.println("Finished finite rotation.");
  } else if (mode == "C" || mode == "c") {
    // Continuous mode: run until user types "exit" into Serial Monitor.
    Serial.println("Running continuous mode. Type 'exit' to stop.");
    while (true) {
      // Check if user has entered a command to exit
      if (Serial.available() > 0) {
        String exitCmd = Serial.readStringUntil('\n');
        exitCmd.trim();
        if (exitCmd.equalsIgnoreCase("exit")) {
          Serial.println("Exiting continuous mode.");
          break;
        }
      }
      digitalWrite(pulPin, HIGH);
      delayMicroseconds(pulseDuration);
      digitalWrite(pulPin, LOW);
      delayMicroseconds(pulseDuration);
    }
  }
  
  delay(100);
}

