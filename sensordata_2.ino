#include <SFE_BMP180.h>  // Include the BMP180 library for interacting with the BMP180 pressure sensor.
#include <Wire.h>        // Include the Wire library for I2C communication.
#include <dht.h>         // Include the DHT library for interacting with the DHT11 temperature and humidity sensor.

dht DHT;                 // Create an instance of the DHT class.

#define DHT11_PIN 7      // Define the pin to which the DHT11 sensor is connected.

SFE_BMP180 pressure;     // Create an instance of the SFE_BMP180 class for the BMP180 pressure sensor.
#define ALTITUDE 1655.0  // Define the altitude at which measurements are being taken.

void setup()
{
  Serial.begin(9600);   // Initialize serial communication at a baud rate of 9600.
  Serial.println("REBOOT");

  if (pressure.begin())  // Initialize communication with the BMP180 sensor.
    Serial.println("BMP180 init success");  // Print a success message if initialization is successful.
  else
  {
    Serial.println("BMP180 init fail\n\n");  // Print an error message if initialization fails.
    while(1);  // Enter an infinite loop, halting the program.
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");

  int chk = DHT.read11(DHT11_PIN);  // Read temperature and humidity from the DHT11 sensor.
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);   // Print the temperature reading from the DHT11 sensor.
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);      // Print the humidity reading from the DHT11 sensor.
  delay(1000);

  status = pressure.startTemperature();  // Start temperature measurement with the BMP180 sensor.
  if (status != 0)
  {
    delay(status);  // Wait for the measurement to complete.

    status = pressure.getTemperature(T);  // Get the temperature reading from the BMP180 sensor.
    if (status != 0)
    {
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");

      status = pressure.startPressure(3);  // Start pressure measurement with oversampling ratio 3.
      if (status != 0)
      {
        delay(status);  // Wait for the measurement to complete.

        status = pressure.getPressure(P,T);  // Get the pressure reading from the BMP180 sensor.
        if (status != 0)
        {
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          p0 = pressure.sealevel(P,ALTITUDE);  // Calculate sea-level pressure.
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          a = pressure.altitude(P,p0);  // Calculate altitude based on pressure readings.
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(5000);  // Delay for 5 seconds before taking the next set of measurements.
}
