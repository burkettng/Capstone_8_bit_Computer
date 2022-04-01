/*
 * Hello!!! I am still learning how to program this my CPU with an 
 * Arduino. Now I have a fully functioning Arduino EEPROM programmer. 
 * Below I am just messing around with some bit shifting.
 * 
 * Class - Senior Capstone
 * Author - Nicholas Burkett
 * Date - 03/31/2022
 */
  
///////////////////////////////////////////////////////
//////////  Define a few important pins  //////////////
///////////////////////////////////////////////////////

#define SHIFT_DATA 2           //Give a name to pin D2
#define SHIFT_CLK 3            //Give a name to pin D3
#define SHIFT_LATCH 4          //Give a name to pin D4

/*
 * Here I need to create a method setAddress() that I can call 
 * in order to shift the bytes accordingly
 * 
 * @param int address - The sequence of bits coming in
 * @param boolean outputEnable - The status of output enable pin 
 * @return - void
 */
void setAddress(int address, bool outputEnable){
  
    //Here I call the Arduino's function shiftOut() to shift a byte one bit at a time
    //The shiftOut() method only shifts one byte at a time!!!
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, ((address >> 8) | (outputEnable ? 0x00 : 0x80)));    //This only shifts out the upper 8 bits
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);                                             //This will only shift the lower 8 bits... ignores the upper 8
    
    digitalWrite(SHIFT_LATCH, LOW);     //Need to toggle my shift latch low
    digitalWrite(SHIFT_LATCH, HIGH);    //Need to toggle my shift latch high
    digitalWrite(SHIFT_LATCH, LOW);     //Need to toggle my shift latch low  
}

/*
 * Here I need to create a method setAddress() that I can call 
 * in order to shift the bytes accordingly
 * 
 * @param int address - The sequence of bits coming in
 * @param boolean outputEnable - The status of output enable pin 
 * @return - void
 */
void setAddress(int address, bool outputEnable){
  
    //Here I call the Arduino's function shiftOut() to shift a byte one bit at a time
    //The shiftOut() method only shifts one byte at a time!!!
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, ((address >> 8) | (outputEnable ? 0x00 : 0x80)));    //This only shifts out the upper 8 bits
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);                                             //This will only shift the lower 8 bits... ignores the upper 8
    
    digitalWrite(SHIFT_LATCH, LOW);     //Need to toggle my shift latch low
    digitalWrite(SHIFT_LATCH, HIGH);    //Need to toggle my shift latch high
    digitalWrite(SHIFT_LATCH, LOW);     //Need to toggle my shift latch low  
}

/*
 * Here is the setup() function that you need for initalizing any 
 * variable I might need for my program
 * 
 * NOTE: This will only run ONCE!!!!
 */
void setup() {

  pinMode(SHIFT_DATA, OUTPUT);         //Set put the SHIFT_DATA pin as output
  pinMode(SHIFT_CLK, OUTPUT);          //Set put the SHIFT_DATA pin as output
  pinMode(SHIFT_LATCH, OUTPUT);        //Set put the SHIFT_DATA pin as output

}

/*
 * Here is the loop() function that I will need for actually running whatever 
 * process I need to run for my program
 * 
 * NOTE: This is where majority of the actual work goes on
 */
void loop() {
  // put your main code here, to run repeatedly:

}
