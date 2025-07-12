int PWM1=1;
int INA1=3;
int INB1=5;
int PWM2=11;
int INA2=9;
int INB2=7;

void setup() { 
 Serial.begin(9600); 
 pinMode(PWM1, OUTPUT); 
 pinMode(INA1, OUTPUT); 
 pinMode(INB1, OUTPUT); 
 pinMode(PWM2, OUTPUT); 
 pinMode(INA2, OUTPUT); 
 pinMode(INB2, OUTPUT); 
 // Set direction to forward 
 digitalWrite(INA1, LOW); 
 digitalWrite(INB1, HIGH); 
 digitalWrite(INA2, LOW); 
 digitalWrite(INB2, HIGH); 
 Serial.println("Enter motor speed (32 to 255):"); 
} 
void loop() { 
 if (Serial.available() > 0) { 
 String input = Serial.readStringUntil('\n'); 
 input.trim(); 
 int speed = input.toInt(); 
 if (speed >= 32 && speed <= 255) {  
    analogWrite(PWM1, speed); 
    analogWrite(PWM2, speed); 
    Serial.print("Motors running at speed: "); 
    Serial.println(speed); 
 } else { 
 Serial.println("Invalid speed. Enter a value between 32 and 255."); 
 } 
 } 
}
