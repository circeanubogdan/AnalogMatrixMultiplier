#include "DFRobot_MCP4725.h"
#include "SPI.h"

#define REF_VOLTAGE 5000

// Intre 18-20 ohm per pas + 100
#define PIN_CS_1 10
#define PIN_CS_2 9
#define PIN_CS_3 8
#define PIN_CS_4 7

#define PIN_SCK 13
#define PIN_SDI 11    // MOSI
#define PIN_SDO 12    // MISO 

#define IN_1 6
#define IN_2 5

DFRobot_MCP4725 DAC1;
DFRobot_MCP4725 DAC2;

uint16_t output_voltage1 = 3000;
uint16_t output_voltage2 = 2000;

void setup() {
  Serial.begin(9600);
  
  DAC1.init(MCP4725A0_IIC_Address0, REF_VOLTAGE);
  DAC2.init(MCP4725A0_IIC_Address1, REF_VOLTAGE);

  pinMode(PIN_CS_1, OUTPUT);
  pinMode(PIN_CS_2, OUTPUT);
  pinMode(PIN_CS_3, OUTPUT);
  pinMode(PIN_CS_4, OUTPUT);

//  pinMode(IN_1, INPUT);
//  pinMode(IN_2, INPUT);

  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
}

void setValue(int l, int value)
// sends value 'value' to SPI device on CS digital out pin 'l'
{
  digitalWrite(l, LOW);
  SPI.transfer(0); // send command byte
  SPI.transfer(value); // send value (0~255)
  digitalWrite(l, HIGH);
}

// Transform number 1-100 to step 0-255

int val_to_step_digipot(int x) {
  float y = 0.0002 + (float) (x - 1) * 0.0098 / 99;

  float R = 1 / y;

  int val = (int) (R - 100) * 51 / 950;
  
  return val;
}

int current_to_val(float y) {
  return (int) ((y - 0.0002) * 99 / 0.0098 + 1);
}

int val_to_voltage(int x) {
  return x / 20;
}

void loop() {

  Serial.print("From ");
  Serial.print(15);
  Serial.print(" to ");
  Serial.println(val_to_step_digipot(15));

  Serial.println(current_to_val(0.0137));

  // put your main code here, to run repeatedly:
//  Serial.print("Output Voltage1:");
//  Serial.print(output_voltage1);
//
//  Serial.print("   Output Voltage2:");
//  Serial.print(output_voltage2);
//  Serial.print("\n");
//
  DAC1.outputVoltage(output_voltage1);
  DAC2.outputVoltage(output_voltage2);



  setValue(PIN_CS_1, val_to_step_digipot(25));
  Serial.print("Resistance value 1:");
  Serial.print(val_to_step_digipot(15));
  Serial.print("\n");

  setValue(PIN_CS_2, val_to_step_digipot(36));
  Serial.print("Resistance value 2:");
  Serial.print(val_to_step_digipot(15));
  Serial.print("\n");

  setValue(PIN_CS_3, val_to_step_digipot(50));
  Serial.print("Resistance value 3:");
  Serial.print(val_to_step_digipot(15));
  Serial.print("\n");

  setValue(PIN_CS_4, val_to_step_digipot(15));
  Serial.print("Resistance value 4:");
  Serial.print(val_to_step_digipot(15));
  Serial.print("\n");

//  int value_1 = digitalRead(IN_1);
//  int value_2 = digitalRead(IN_2);

//  Serial.println(value_1-value_2);

  delay(15000);

}
