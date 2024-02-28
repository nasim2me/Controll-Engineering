int a=9;
int b=10;
long c;
int d;
void setup()
{
  pinMode(a,OUTPUT);
  pinMode(b, INPUT);
  Serial.begin(9600); 
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
