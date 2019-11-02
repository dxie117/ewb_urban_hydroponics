//This code was written to be easy to understand.
									 
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the RTD temperature circuit.
//This code was written in the Arduino 1.8.9 IDE
//An Arduino UNO was used to test this code.
//This code was last tested 6/2019


#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it

//Testing pin assignments
//Arduino RX: 2
//Arduino TX: 3

//Rx port of circuit connects to tx port of Arduino
//Wire circuit rx port to Arduino Digital Pin 3

//Tx port of circuit connects to rx port of Arduino
//Wire circuit tx port to Arduino Digital Pin 2

//define what pin rx is going to be
#define rx 2

//define what pin tx is going to be
#define tx 3


SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float temperature;                                    //used to hold a floating point number that is the RTD temperature



void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  myserial.begin(9600);                               //set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


void loop() {                                         //here we go...

  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }


  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    Serial.println(sensorstring);                     //send that string to the PC's serial monitor
    if (isdigit(sensorstring[0])) {                   //if the first character in the string is a digit
      temperature = sensorstring.toFloat();           //convert the string to a floating point number so it can be evaluated by the Arduino
      if (temperature >= 25.0) {                      //if the RTD temperature is greater than or equal to 25 C
        Serial.println("high");                       //print "high" this is demonstrating that the Arduino is evaluating the RTD temperature as a number and not as a string
      }
      if (temperature <= 24.999) {                    //if the RTD temperature is less than or equal to 24.999 C
        Serial.println("low");                        //print "low" this is demonstrating that the Arduino is evaluating the RTD temperature as a number and not as a string
      }
    }
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
}
