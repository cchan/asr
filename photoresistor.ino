const int photoresistorPin = 0;  //define a pin for Photo resistor

void setup()
{
  Serial.begin(9600);  //Begin serial communcation
  pinMode( photoresistorPin, INPUT );
}

int i = 0;
void loop()
{
  Serial.println(analogRead(photoresistorPin)); //Write the value of the photoresistor to the serial monitor.
}

