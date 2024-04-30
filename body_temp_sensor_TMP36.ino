// C++ code
//

#define temp A0
void setup()
{
  pinMode(temp, INPUT);
  Serial.begin(9600);
}

void loop()
{
  	Serial.print("Temp: ");
  	double voltage = (analogRead(temp) * 5.0) / 1024;
	Serial.println((voltage - 0.50002) * 100);
}