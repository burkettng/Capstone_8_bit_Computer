/*
 * Hello!!! Here I am playing around with some of my shift registers
 * and understanding how they work. This allows me to access more pins from
 * the arduino onto my 8 bit computer... Or atleast that is how I understand
 * it at the moment. 
 * 
 * This is my first program that I am writting towards my 8-bit computer
 * Class - Senior Capstone 
 * Author - Nicholas Burkett
 * Date - 03/31/2022
 */
 
 
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
 

void setup() {

  pinMode(SHIFT_DATA, OUTPUT);         //Set put the SHIFT_DATA pin as output
  pinMode(SHIFT_CLK, OUTPUT);          //Set put the SHIFT_DATA pin as output
  pinMode(SHIFT_LATCH, OUTPUT);        //Set put the SHIFT_DATA pin as output

  setAddress(1234, true);              //Call the method setAddress()

}

void loop() {
  // put your main code here, to run repeatedly:

}
