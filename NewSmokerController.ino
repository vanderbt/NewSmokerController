
#include <Console.h>
#include <Bridge.h>
//#include <YunServer.h>
#include <Servo.h>
#include <HttpClient.h>
#include <BridgeClient.h>
#include "Thermistor4.h"
#include <PID_v1.h>


#define THERMISTORPinSmoker 0
#define THERMISTORPinMeat 1



/***************************************************Variable Declarations*******************************************/
String smokerTemp;
String stringQuery;
String  parameters = "";

char charBuf[25];
char setTemp1[4];

int q = 0;
int SmokerSave;
int lidOffCounter = 1;

//Define PID Inputs
double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
double aggKp=8, aggKi=0.5, aggKd=4;
double consKp=1, consKi=0.05, consKd=0.25;

Thermistor4 ThermistorSmoker, ThermistorMeat;

/********************************************Instantiating Classes***********************************/

BridgeClient client;
HttpClient client1;
IPAddress server(54,213, 180, 34);
Servo myservo;
PID myPID(&Input, &Output, &Setpoint,aggKp,aggKi,aggKd, DIRECT);


/*******************************************************************Setup Function***********************************/
void setup()
{
  Bridge.begin();
  myservo.attach(9);
  
int Smoker = ThermistorSmoker.GetFarenheit();
SmokerSave = ThermistorSmoker.GetFarenheit();
  Input = Smoker;
//Set Setpoint
Setpoint = 200;

/**************Set PID to Automatic***********/
  myPID.SetMode(AUTOMATIC);
/*****Still not certain how this applies. I'm researching*****/ 
  myPID.SetSampleTime(100);
      
//Thermistor configuration and calibration

  ThermistorSmoker.Pin = THERMISTORPinSmoker;           //Set the pin number.
  ThermistorMeat.Pin = THERMISTORPinMeat;
  ThermistorSmoker.SetUp();                             //Sets up the analog read pin for internal AVR.
  ThermistorMeat.SetUp();                               //If using an external ADC, write your own setup here.

//pow() is used elsewhere so might as well be used here.

  ThermistorSmoker.BitResolution=pow(2, 10)-1;          //ATmega's have a 10bit ADC (2^10-1).
  ThermistorMeat.BitResolution=pow(2, 10)-1;            //An external ADC may be lower or higher than 10bits.
  ThermistorSmoker.VoltageSupply=5.03;                  //My USB powers my ATmega325 chip at 4.95v. Meter this for accuracy.
  ThermistorMeat.VoltageSupply=5.03;                    //An external ADC may have different voltages. Meter this for accuracy.
  ThermistorSmoker.ResistanceFixed=10030;               //Fixed resistor in the divider. Measured in ohms. Meter this for accuracy.
  ThermistorMeat.ResistanceFixed=10030;                 //The resistor divider should be calculated to maximize desired range.
  ThermistorSmoker.Offset=1.25;                          //adjust temperature in Kelvin up or down a little to account for unforseen variations.
  ThermistorMeat.Offset=1.25;                            //This will be by trial and error during final manual calibration.

//These numbers were generated from thermistor.sf.net and aren't quite right unless using the full equation.

  ThermistorSmoker.SteinhartA1=1.987798628427711e-003;  //First Steinhart-Hart coefficient.
  ThermistorSmoker.SteinhartA2=-3.024028913435313e-004; //Second Steinhart-Hart coefficient.
  ThermistorSmoker.SteinhartA3=5.328334158681741e-005;  //Third Steinhart-Hart coefficient.
  ThermistorSmoker.SteinhartA4=-1.575690075905493e-006; //Fourth Steinhart-Hart coefficient.
  ThermistorMeat.SteinhartA1=1.987798628427711e-003;    //First Steinhart-Hart coefficient.
  ThermistorMeat.SteinhartA2=-3.024028913435313e-004;   //Second Steinhart-Hart coefficient.
  ThermistorMeat.SteinhartA3=5.328334158681741e-005;    //Third Steinhart-Hart coefficient.
  ThermistorMeat.SteinhartA4=-1.575690075905493e-006;   //Fourth Steinhart-Hart coefficient.
}


/*************************************************************Primary Loop**************************************/
void loop(){
/*******************************************PID****************************************************************/ 
    Setpoint = getSetPoint();   //Get Setpoint Function;
 
    /**************Get Smoker and Meat Temperature and Convert to Integer*********/
    ThermistorSmoker.ReadCalculate(3);
    int Smoker = ThermistorSmoker.GetFarenheit();
    ThermistorMeat.ReadCalculate(3);
    int Meat = ThermistorMeat.GetFarenheit();

    /**************Lid Off Algorithm
     * This uses counter q, which is incremented every 500ms to check
     * the temperature every 4 seconds and looks for a 4% drop in temperature.
     * If the condition is met, controller operation is suspended for 2
     * minutes. Note:Need to figure out how to suspend controller but continue
     * to update cloud.
     */
    if (q >= 7) {
        q = 0;
      if (Smoker <= SmokerSave * .96){
        //Sets fan speed to zero
        analogWrite(3, 0);
        Output = 0;
        myPID.SetMode(MANUAL);
        lidOffCounter = 240;
        /*Console.println("lidOffCounter Value:");
        Console.println(lidOffCounter);
        //Debug output
        Console.println("Condition Met");
        Console.print("Current Smoker Temp "); 
        Console.println(Smoker);
        Console.print("Saved Smoker Temp ");
        Console.println(SmokerSave);
        */
       
      }
    }
    if (lidOffCounter == 0) {
      myPID.SetMode(AUTOMATIC);
    }
      
    Input = Smoker;
    char buf[4];
    delay(1);
    
    //Adaptive tuning
    double gap = abs(Setpoint-Input); //distance away from setpoint
      if(gap<2)
        {  //we're close to setpoint, use conservative tuning parameters
            myPID.SetTunings(consKp, consKi, consKd);
        }
        else
        {
          //we're far from setpoint, use aggressive tuning parameters
          myPID.SetTunings(aggKp, aggKi, aggKd);
        }
    /*
     * Compute PID and send output to fan. Write PWM to Fan, Range 0-255
     */
    myPID.Compute();
    analogWrite(3,Output);
    
    /****************Maps Output to Servo range of 2 to 90 degrees.*********/
    int MappedOutput = map(Output,0, 255, 90, 2);
    myservo.write(MappedOutput);

    /****************On first pass of loop, save smoker temperature for lid off algorithm*********/
    if (q == 0) {
      SmokerSave = Smoker;
    }
    q++;
    
    if (lidOffCounter == 240){
    lidOffCounter--;
    }
    lidOffCounter--;
    if (lidOffCounter == -1) {
      lidOffCounter = 239;
    }
    sendToCloud(Output, Smoker, Meat);
}

/*********************************Send to Cloud***********************************************************
 * ******************************************************************************************************* 
 */
void sendToCloud(int O, int S, int M) {

/*********Converts Meat Temp, Smoker Temp and Fan Speed to format for posting to Cloud and Posts****/  
  
  /*************This maps 0 to 255 to 0 to 100 to show fan speed in percent for display on cloud*******/

  
  int MappedFanSpeed = map(O, 0, 255, 0, 100);
  int myFloatOutput = MappedFanSpeed;

  /***************Converts values to strings so that they can be sent to the cloud as a URL***********************/
  char bufferSmoker[6];
  char bufferMeat[6]; 
  char bufferFan[6];

  String smokerTemp = dtostrf(S, 1, 0, bufferSmoker);
  String meatTemp = dtostrf(M, 1, 0, bufferMeat);
  String fanSpeed = dtostrf(myFloatOutput, 1, 0, bufferFan);

/*************Sends data to cloud*********************************************************************************/
if (client.connect(server, 80)) {
                         
                          delay(500);
                
                          parameters= "d=" + String(smokerTemp) + "&e=" + String(meatTemp) + "&f=" + String(fanSpeed);
                          /***********add.php writes data to mySQL database on cloud server************************/
                          client.println("POST /smokerYun/add.php HTTP/1.1");
                          client.println("Host: 54.213.180.34");
                          client.print("Content-length:");
                          client.println(parameters.length());
                          client.println("Connection: Close");
                          client.println("Content-Type: application/x-www-form-urlencoded;");
                          client.println();
                          client.println(parameters);                                        
                 }else{
                        Serial.println("connection failed");           
                        delay(10);
                 }
                 if(client.connected()){
                             client.stop();   //disconnect from server
                 }
}
/*********************************Get Setpoint************************************************************
 * ******************************************************************************************************* 
 */
int getSetPoint(){
/*************************************Get Set Point From Amazon Cloud*****************************************/

                        client1.get("http://54.213.180.34/smokerYun/get.php");
                           char c = 70;
                           int count = 0;
                           char setTemp1[] = {0}; 
                            while(client1.available()) {
                              char c = client1.read();
                              setTemp1[count] = c;
                              //Console.println(setTemp1[count]);
                              count++;
                            }            
                            int sP = atoi(setTemp1);
                            return sP;
                            Console.println(sP);
}


