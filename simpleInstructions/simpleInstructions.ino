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

//Here I define the individual bits that make up the control word

#define HLT 0b1000000000000000  //The encoding for the Halt bit
#define RI 0b0010000000000000   //The encoding for the RAM IN bit
#define IO 0b0000100000000000   //The encoding for the Instruction Register Out bit
#define AI 0b0000001000000000   //The encoding for the A Register In bit
#define AO 0b0000000100000000   //The encoding for the A Register Out bit
#define EO 0b0000000010000000   //The encoding for the Sum Out bit
#define SU 0b0000000001000000   //The encoding for the Subtract bit on the ALU
#define BI 0b0000000000100000   //The encoding for the B Register In bit
#define OI 0b0000000000010000   //The encoding for the Output Register In bit
#define J  0b0000000000000100   //The encoding for the Jump bit            

  /****** The four micro insturcions make up the fetch command ******/
#define MI 0b0100000000000000   //The encoding for the Memory Address Register IN bit!
#define CO 0b0001010000001000   //The encoding for the Counter Out bit
#define RO 0b0001000000000000   //The encoding for the RAM Out bit
#define II 0b0000010000000000   //The encoding for the Instruction Register IN bit 
#define CE 0b0000000000001000   //The encoding for the Counter Enable bit
/****************************************************************************************/

//Now I can write the micro code using these constants!!!!!!!


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


//Here is where I deleted the code that would program an EEPROM to intertpret binary into decimal!!!!
//I will now be programming the EEPROM to interpret the logic that will run the CPU.

/****** Notice how each instruction starts off with the FETCH instruction!!! ******/
uint_16 dataArr[]= {
  
        MI|CO, RO|II|CE, 0, 0, 0, 0, 0, 0               //0000 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO, MI, RO, AI, 0, 0, 0, 0     //0001 - LDA... encoding for the load A inst.
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //0010 - ADD... encoding for the ADD inst.l 

        //The commands below I have not defined yet
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //0011 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //0100 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //0101 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //0110 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //0111 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1000 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1001 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1010 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1011 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1100 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1101 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1110 - NOP... encoding for inst. no operation
        MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI, 0, 0, 0   //1111 - NOP... encoding for inst. no operation
        
};

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
