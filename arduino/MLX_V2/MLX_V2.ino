/*
2-16-2013
Spark Fun Electronics
Nathan Seidle
This code is heavily based on maxbot's and IlBaboomba's code: http://arduino.cc/forum/index.php?topic=126244
They didn't have a license on it so I'm hoping it's public domain.
This example shows how to read and calculate the 64 temperatures for the 64 pixels of the MLX90620 thermopile sensor.
alpha_ij array is specific to every sensor and needs to be calculated separately. Please see the
'MLX90620_alphaCalculator' sketch to get these values. If you choose not to calculate these values
this sketch will still work but the temperatures shown will be very inaccurate.
Don't get confused by the bottom view of the device! The GND pin is connected to the housing.
To get this code to work, attached a MLX90620 to an Arduino Uno using the following pins:
A5 to 330 ohm to SCL
A4 to 330 ohm to SDA
3.3V to VDD
GND to VSS
I used the internal pull-ups on the SDA/SCL lines. Normally you should use ~4.7k pull-ups for I2C.
*/

/*
Modified code of MLX by Kevin Arias for Swarm search and rescue robot.
*/


//BBB interface constants
const int BBB_PWR = 2;
const int SYS_PWR = 3;
const int LED = 13;


#include <i2cmaster.h>
#include "MLX90620_registers.h"

int refreshRate = 32; //Set this value to your desired refresh frequency
bool state = 0;

//Global variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
byte readingData[132]; //Contains the raw IR data from the sensor
byte eepromData[256]; //Contains the full EEPROM reading from the MLX (Slave 0x50)
//These are constants calculated from the calibration data stored in EEPROM
//See varInitialize and section 7.3 for more information
byte loopCount = 0; //Used in main loop
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Begin Program code

void setup()
{
    Serial.begin(57600);
    pinMode(SYS_PWR, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(BBB_PWR, INPUT);

    digitalWrite(SYS_PWR, HIGH);    // Enables power of the system 
    digitalWrite(LED,HIGH);   // Power on system Led
    delay(1000); // Wait for BBB initialization
    digitalWrite(LED , LOW);    // Enables power of the system 

    
    waitPulse();
    digitalWrite(LED,HIGH);   // Power on system Led


    i2c_init(); //Init the I2C pins
    PORTC = (1 << PORTC4) | (1 << PORTC5); //Enable pull-ups
    delay(5); //Init procedure calls for a 5ms delay after power-on
    
    read_EEPROM_MLX90620(); //Read the entire EEPROM
    setConfiguration(refreshRate); //Configure the MLX sensor with the user's choice of refresh rate

    //Serial.write(eepromData,255);
   
    digitalWrite(LED,LOW);   // Power on system Led
   
   
}


void loop()
{

    waitPulse();
    digitalWrite(LED,HIGH); 

    if(checkConfig_MLX90620()) //Every 16 readings check that the POR flag is not set
        setConfiguration(refreshRate); //Re-write the configuration bytes to the MLX

    readPTAT_MLX90620();      
    readIR_MLX90620(); //Get the 64 bytes of raw pixel data into the irData array
    readCPIX_MLX90620(); //Go get the raw data of the compensation pixel
    Serial.write(readingData,132);
    
    
    digitalWrite(LED,LOW); 

    
    //calculate_TA(); //Calculate the new Tambient
   //calculate_TO(); //Run all the large calculations to get the temperature data for each pixel
    //prettyPrintTemperatures(); //Print the array in a 4 x 16 pattern
    //rawPrintTemperatures(); //Print the entire array so it can more easily be read by Processing app

}



void waitPulse()
{
  
  
    while(state)   // while powering on BBB
      state=digitalRead(BBB_PWR); //keep reading
    
    while(!state) //waits the start signal from BBB
      state=digitalRead(BBB_PWR); 
}


//Receives the refresh rate for sensor scanning
//Sets the two byte configuration registers
//This function overwrites what is currently in the configuration registers
//The MLX doesn't seem to mind this (flags are read only)
void setConfiguration(int irRefreshRateHZ)
{
  byte Hz_LSB;
  switch(irRefreshRateHZ)
  {
      case 0:
      Hz_LSB = 0b00001111;
      break;
      case 1:
      Hz_LSB = 0b00001110;
      break;
      case 2:
      Hz_LSB = 0b00001101;
      break;
      case 4:
      Hz_LSB = 0b00001100;
      break;
      case 8:
      Hz_LSB = 0b00001011;
      break;
      case 16:
      Hz_LSB = 0b00001010;
      break;
      case 32:
      Hz_LSB = 0b00001001;
      break;
      case 64:
      Hz_LSB = 0b00001000;
      break;
      default:
      Hz_LSB = 0b00001110;
  }
  byte defaultConfig_H = 0b01110100; // x111.01xx, Assumes NA = 0, ADC low reference enabled, Ta Refresh rate of 2Hz
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(0x03); //Command = configuration value
  i2c_write((byte)Hz_LSB - 0x55);
  i2c_write(Hz_LSB);
  i2c_write(defaultConfig_H - 0x55); //Assumes NA = 0, ADC low reference enabled, Ta Refresh rate of 2Hz
  i2c_write(defaultConfig_H);
  i2c_stop();
}


//Read the 256 bytes from the MLX EEPROM and setup the various constants (*lots* of math)
//Note: The EEPROM on the MLX has a different I2C address from the MLX. I've never seen this before.
void read_EEPROM_MLX90620()
{
  i2c_start_wait(MLX90620_EEPROM_WRITE);
  i2c_write(0x00); //EEPROM info starts at location 0x00
  i2c_rep_start(MLX90620_EEPROM_READ);
  //Read all 256 bytes from the sensor's EEPROM
  for(int i = 0 ; i <= 255 ; i++)
    eepromData[i] = i2c_readAck();
  i2c_stop(); //We're done talking

  writeTrimmingValue(eepromData[OSC_TRIM_VALUE]);
}


//Given a 8-bit number from EEPROM (Slave address 0x50), write value to MLX sensor (Slave address 0x60)
void writeTrimmingValue(byte val)
{
  i2c_start_wait(MLX90620_WRITE); //Write to the sensor
  i2c_write(0x04); //Command = write oscillator trimming value
  i2c_write((byte)val - 0xAA);
  i2c_write(val);
  i2c_write(0x56); //Always 0x56
  i2c_write(0x00); //Always 0x00
  i2c_stop();
}





//Reads the PTAT data from the MLX
//Returns an unsigned int containing the PTAT
unsigned int readPTAT_MLX90620()
{
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(CMD_READ_REGISTER); //Command = read PTAT
  i2c_write(0x90); //Start address is 0x90
  i2c_write(0x00); //Address step is 0
  i2c_write(0x01); //Number of reads is 1
  i2c_rep_start(MLX90620_READ);
  readingData[0] = i2c_readAck(); //Grab the lower and higher PTAT bytes
  readingData[1] = i2c_readAck();
  i2c_stop();
  //return( (unsigned int)(ptatHigh << 8) | ptatLow); //Combine bytes and return
}





//Reads 64 bytes of pixel data from the MLX
//Loads the data into the irData array
void readIR_MLX90620()
{
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(CMD_READ_REGISTER); //Command = read a register
  i2c_write(0x00); //Start address = 0x00
  i2c_write(0x01); //Address step = 1
  i2c_write(0x40); //Number of reads is 64
  i2c_rep_start(MLX90620_READ);
  for(int i = 2 ; i < 130 ; i++ )
  {
    readingData[i] = i2c_readAck();
  }
  i2c_stop();
 // irData[i] = (int)(pixelDataHigh << 8) | pixelDataLow;
}


//Read the compensation pixel 16 bit data
int readCPIX_MLX90620()
{
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(CMD_READ_REGISTER); //Command = read register
  i2c_write(0x91);
  i2c_write(0x00);
  i2c_write(0x01);
  i2c_rep_start(MLX90620_READ);
  readingData[130] = i2c_readAck(); //Grab the two bytes
  readingData[131] = i2c_readAck();
  i2c_stop();
  //return ( (int)(cpixHigh << 8) | cpixLow);
}


//Reads the current configuration register (2 bytes) from the MLX
//Returns two bytes
unsigned int readConfig_MLX90620()
{
  i2c_start_wait(MLX90620_WRITE); //The MLX configuration is in the MLX, not EEPROM
  i2c_write(CMD_READ_REGISTER); //Command = read configuration register
  i2c_write(0x92); //Start address
  i2c_write(0x00); //Address step of zero
  i2c_write(0x01); //Number of reads is 1
  i2c_rep_start(MLX90620_READ);
  byte configLow = i2c_readAck(); //Grab the two bytes
  byte configHigh = i2c_readAck();
  i2c_stop();
  return( (unsigned int)(configHigh << 8) | configLow); //Combine the configuration bytes and return as one unsigned int
}


//Poll the MLX for its current status
//Returns true if the POR/Brown out bit is set
boolean checkConfig_MLX90620()
{
  if ( (readConfig_MLX90620() & (unsigned int)1<<POR_TEST) == 0)
    return true;
  else
    return false;
}



