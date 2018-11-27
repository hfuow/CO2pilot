#include <SenseairBlgK33.h>
#include <SdFat.h>

BLG object;
const uint8_t chipSelect = 4;
SdFat sd; // sd object to read the data in SD card
SdFile file; // file to write the data in 
void setup() {
  Serial.begin(9600);
  object.begin();
  pinMode(5, OUTPUT); // analog pin, connected with sensor to I2C communication
  analogWrite(5, 255);
  Serial.println("RESET");
  Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
  pinMode(10,OUTPUT);
  digitalWrite(10, HIGH);
  if (!sd.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    sd.initErrorHalt();
    }
  Serial.println("card initialized.");
}

void loop() {
  Serial.println("Request reading");
  object.wakeSensor();
  object.initPoll();
  
  Serial.println("wait 16s for reading ");
  delay(16000);
  
  object.wakeSensor();
  double CO2 = object.readCO2(); 
  object.wakeSensor();
  double Temp = object.readTemp();
  object.wakeSensor();
  double Rh = object.readRh();
  
  Serial.print("CO2: ");Serial.print(CO2);Serial.print(" ppm, ");
  Serial.print("Temp: ");Serial.print(Temp);Serial.print(" C, ");
  Serial.print("Rh: ");Serial.print(Rh);Serial.println(" %");

  if (!file.open("data.csv", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }

  file.print((String(CO2)) + "," + String(Temp) + "," + String(Rh));
  file.println();
  file.close();
  Serial.println("wait further 9s to cool down"); // keep >25s between initPoll()
  delay(9000);
}

