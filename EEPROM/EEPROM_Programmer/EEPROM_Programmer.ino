/*
 * Hello!!!! Here I am building a EEPROM programmer from an adruino. 
 * I will be using this to program my EEPROM that will run on my 8-bit computer
 * Throughout this file you will see helper functions that I will be using a good bit
 * 
 * Class - Senior Capstone
 * Version 2
 * Date 3/31/2022
 */
 
//////////////////////////////////////////////////////////////////////////
//////  Here I need to declare/initialize a few constant variables  //////
//////////////////////////////////////////////////////////////////////////

#define SHIFT_DATA 2      //Declare data pin 2 on Arduino
#define SHIFT_CLK 3       //Declare clock pin 3 on Arduino
#define SHIFT_LATCH 4     //Declare latch pin 4 on Arduino
#define EEPROM_D0 5       //Declare data pin 5 on Arduino
#define EEPROM_D7 12      //Declare data pin 7 on Arduino
#define WRITE_EN 13       //Declare write enable 13 two on Arduino

/*
 * Here is the setAddress() function that will set the address of the given data
 * 
 * @param int address - the data coming in... only grab a byte at a timen using shift registers!
 * @param bool outputEnable - true/false if need to set output enable
 * @return - void
 */
void setAddress(int address, bool outputEnable){
  
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));     //Grab the upper 8 bits
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);                                           //Grab the lower 8 bits

  digitalWrite(SHIFT_LATCH, LOW);            //Set the SHIFT_LATCH pin to low
  digitalWrite(SHIFT_LATCH, HIGH);           //Set the SHIFT_LATCH pin to high
  digitalWrite(SHIFT_LATCH, LOW);            //Set the SHIFT_LATCH pin to low
}


/*
 * Here I will implement a readEEPROM function that will read 
 * a byte of data from a specified address
 * 
 * @param int address - address of given data to read
 * @return byte - return the byte of memory read
 */
byte readEEPROM(int address){

  //Loop through the pins on the Arduino
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    
    pinMode(pin, INPUT);    //Set each pin as input
  }
  
  setAddress(address, /*outputEnable*/ true);     //Call set address 

  byte data = 0;    //Start data out as 0

  //Loop through each byte bit by bit
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1){
    
    data = (data << 1) + digitalRead(pin);    //Set the data... aka the byte to return
  }
  
  return data;    //Return the byte of data
}


/*
 * Here I will implement a writeEEPROM() that will allow me to 
 * write to a byte of memory on my chip at some specified address:).
 * I will do this using my shift registers
 * 
 * @param int address - the address of the byte of memory to write to
 * @param byte data - the data to write
 */
void writeEEPROM(int address, byte data){
  
  setAddress(address, /*outputEnable*/ false);    //Call setAddress()

  //Loop through each pin 
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1){
    
    pinMode(pin, OUTPUT);   //Set each pin as OUTPUT and not INPUT
  }

  //Loop through each byte bit by bit
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1){
    
    digitalWrite(pin, data & 1);      //Write the intended data
    data = data >> 1;                 //Ajust data accordingly
  }
      
  digitalWrite(WRITE_EN, LOW);        //Set the write enable pin to low
  delayMicroseconds(1);               //Delay the clocl 1 microsecond
  digitalWrite(WRITE_EN, HIGH);       //Set the write enable pin to low
  delay(10);                          //Delay the clocl 10 milisecond
}


/*
 * Here is a standard print function that will print the contents of 
 * the memory... just the first 255 mem addresses
 * 
 * @return void
 */
void printContents(){

  //Loop through the memory addresses
  for (int base = 0; base <= 255; base += 16){
    
    byte data[16];    //Declare a data array of 16 items

    //Loop through each bit
    for (int offset = 0; offset <= 15; offset += 1){
      
      data[offset] = readEEPROM(base + offset);   //Ajust data accordingly
    }

    char buf[80];   //Create char array
    
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);    //Print out buf
  }
}


// 4-bit hex decoder for common anode 7-segment display
//byte data[] = { 0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84, 0x88, 0xe0, 0xb1, 0xc2, 0xb0, 0xb8 };

// 4-bit hex decoder for common cathode 7-segment display
 byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };


/*
 * Here is the setup() function for the file... Here I delcare and initalize 
 * needed variables and such that I will need ot run my program
 * 
 * @return void
 */
void setup(){
  
  pinMode(SHIFT_DATA, OUTPUT);              //Set SHIFT_DATA as an output pin 
  pinMode(SHIFT_CLK, OUTPUT);               //Set SHIFT_CLK as an output pin 
  pinMode(SHIFT_LATCH, OUTPUT);             //Set SHIFT_LATCH as an output pin 
  digitalWrite(WRITE_EN, HIGH);             //Set WRITE_EN to HIGH 
  pinMode(WRITE_EN, OUTPUT);                //Set WRITE_EN as an output pin 
  Serial.begin(57600);                      //Set the current baud rate

  // Erase entire EEPROM
  Serial.print("Erasing EEPROM");                                 //Display message to user

  //Loop through the mem addresses
  for (int address = 0; address <= 2047; address += 1){
    
    writeEEPROM(address, 0xff);     //Call writeEEPROM() on desired address

    //Check if address is a multiple of 64
    if (address % 64 == 0){
      
      Serial.print(".");    //If so, print a '.'
    }
  }
  
  Serial.println(" done");    //Display done

  // Program data bytes
  Serial.print("Programming EEPROM");    //Display message to user
  for (int address = 0; address < sizeof(data); address += 1){
    
    writeEEPROM(address, data[address]);      //Write to mem address

    //check if multiple of 64
    if (address % 64 == 0){
      
      Serial.print(".");    //If so, print a '.'
    }
  }
  
  Serial.println(" done");   //Display done

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  
  printContents();    //Call printContents() to check my work
}


void loop() {
  // put your main code here, to run repeatedly:

}
