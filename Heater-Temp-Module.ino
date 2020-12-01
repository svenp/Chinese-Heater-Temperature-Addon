#include <SoftwareSerialWithHalfDuplex.h>
SoftwareSerialWithHalfDuplex sOne(2, 2); // NOTE TX & RX are set to same pin for half duplex operation
int inhibitswitch = 8; // inhibit switch to toggle Auto mode Pin D8 to GND is auto off
unsigned long lasttime;
String heaterstate[] = {"Off","Starting","Pre-Heat","Failed Start - Retrying","Ignition - Now heating up","Running Normally","Stop Command Received","Stopping","Cooldown"};
int heaterstatenum = 0;
int heatererror = 0;
String controlenable = "False";
String inhibitseenactive = "False";
float currtemp = 0;
float settemp = 0;
float command = 0;
void setup()
{
 // initialize inhibit switch
 pinMode(inhibitswitch,INPUT_PULLUP);
 // initialize listening serial port
 // 25000 baud, Tx and Rx channels of Chinese heater comms interface:
 // Tx/Rx data to/from heater, special baud rate for Chinese heater controllers
 sOne.begin(25000);
 // initialise serial monitor on serial port 0
 Serial.begin(115200);
 // prepare for detecting a long delay
 lasttime = millis();

}

void loop()
{
 static byte Data[48];
 static bool RxActive = false;
 static int count = 0;

 
 // read from serial on D2
 if (sOne.available()) {

 // calc elapsed time since last rx’d byte to detect start of frame sequence
 unsigned long timenow = millis();
 unsigned long diff = timenow - lasttime;
 lasttime = timenow;

 if(diff > 100) { // this indicates the start of a new frame sequence
 RxActive = true;
 }
 int inByte = sOne.read(); // read hex byte
 if(RxActive) {
 Data[count++] = inByte;
 if(count == 48) {
 RxActive = false;
 }
 }
 }

 if(count == 48) { // filled both frames – dump
 count = 0;
command = int(Data[2]);
currtemp = Data[3];
settemp = Data[4];
heaterstatenum = int(Data[26]);
heatererror = int(Data[41]);

if (digitalRead(inhibitswitch) == HIGH) {
  inhibitseenactive = "True";
}

if (digitalRead(inhibitswitch) == LOW && (inhibitseenactive == "True")) {
  inhibitseenactive = "False";
  Serial.println("Inhibit Switch Toggled disable-enable - Enabling Auto");
 // controlenable = "True"; // workarroud to disable inhibit switch
}

 Serial.println();
 Serial.print("Command ");
 Serial.println(int(command));
 Serial.println(heaterstate[heaterstatenum]);
 Serial.print("Error Code ");
 Serial.println(heatererror);
 Serial.print("Current Temp ");
 Serial.println(int(currtemp));
 Serial.print("Set Temp ");
 Serial.println(int(settemp));
 Serial.print("System Enabled : ");
 Serial.println(controlenable);
//160 is start command, 5 is stop command for the heater
if (int(command) == 5) {
  Serial.println("Stop command seen from controller - Enabling Auto");
  controlenable = "True";
}
if (int(command) == 160) {
  Serial.println("Start command seen from controller - Disabling Auto");
  controlenable = "False";
}


settemp = 2; //start heater at 0 Celsuis degrees of frost ro 20 to test
  // Standard Range     Temp +2 to Temp -1 (3 degrees)

if (controlenable == "True") {
if (int(settemp) > int(currtemp) && (digitalRead(inhibitswitch) == LOW) && (heatererror <= 1))  {
  Serial.println("Temperature Below Lower Limit");
  Serial.println("Heater should be running ");
  if (heaterstatenum == 0) { // State 0 when heater is off
uint8_t data1[24] = {0x78,0x16,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x32,0x08,0x23,0x05,0x00,0x01,0x2C,0x0D,0xAC,0x8D,0x82};
delay(50);
sOne.write(data1, 24);
  }
}

 if (int(settemp) < (int(currtemp) - 1) && (digitalRead(inhibitswitch) == LOW)) {
  Serial.println("Temperature Above Upper Limit");
  Serial.println("Heater should stop ");
  if (heaterstatenum == 5) { //State 5 when heater is on
uint8_t data1[24] = {0x78,0x16,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x32,0x08,0x23,0x05,0x00,0x01,0x2C,0x0D,0xAC,0x61,0xD6};
delay(50);
sOne.write(data1, 24);
  }
}
}




 

 }
} // loop

// Temperatur Scala 1-10 Led => 8-35 Grad
// LED -> Celsius
// 1 -> 8
// 2 -> 11
// 3 -> 14
// 4 -> 17
// 5 -> 20
// 6 -> 23
// 7 -> 26
// 8 -> 29
// 9 -> 32
// 10 -> 35
