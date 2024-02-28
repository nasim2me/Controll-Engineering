#include<Servo.h>
Servo a;
int b=0;
void setup()
{
  a.attach(9);
 
}
void loop()
{
  for(b=0;b<=180;b+=10)
  {
    a.write(b);
    delay(10);
  }
  for(b=1800;b>=0;b-=10)
  {
    a.write(b);
    delay(10);
  }
}
