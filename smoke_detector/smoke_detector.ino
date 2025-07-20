int mq=A0;
int x;
int led=8;
int buzzerpin=10;
void setup() 
{
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(8,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() 
{
  x=analogRead(A0);
  Serial.println(x);
  delay(500);
  if (x>=100)
  {
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);
  }
  else
  {
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
  }

  
}
