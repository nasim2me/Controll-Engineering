int a=9; // trigPin
int b=10; // echoPin
long c; // duration
int d; // distance
void setup()
{
  pinMode(a,OUTPUT);
  pinMode(b, INPUT);
  Serial.begin(115200); 
}
void loop()
{
  digitalWrite(a, LOW);
  delayMicroseconds(2);
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a,LOW);

  c=pulseIn(b, HIGH);
  d=c*.034/2;

  Serial.print("Distance: ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println();
  delay(1000);
  
}
