/* Dynamixel ID Change Example
 

 */
 /* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
 
 /*Initializations*/
 int desiredAngle = 0;
 int currentAngle = 0;
 int maxSpeed = 125;  
 float error = 0;
 int dark = 1100;
 int white = 350;
 int range = dark - white;
 Dynamixel Dxl(DXL_BUS_SERIAL1); 
 
void setup() {
  
  //SerialUSB.begin(9600);
  // Initialize the dynamixel bus:
  // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps  
  Dxl.begin(3);
  Dxl.setPacketType(DXL_PACKET_TYPE2);
  delay(1000);

  /*Intialize Motor*/
  Dxl.wheelMode(100); //jointMode() is to use position mode 
  delay(1000);
  
  //Setup interrupt for keyboard input
  SerialUSB.attachInterrupt(servoAngle);
 
  /*LED's for error checking*/
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, HIGH);
}

void servoAngle(byte* input, byte ncount){
  digitalWrite(BOARD_LED_PIN, LOW);
  SerialUSB.println();
  char desiredCharAngle[ncount];
  float normalizedError;
  float correctiveSpeed;
  
  for(int i = 0; i < ncount-1; i++){
    desiredCharAngle[i] = char(input[i]);
  }
  
  desiredAngle = atoi(desiredCharAngle); 
  SerialUSB.println('Desired angle changed');
  SerialUSB.println(desiredAngle);
}
 
void loop() {
  digitalWrite(BOARD_LED_PIN, LOW);
  
   /*Control Loop*/
    currentAngle = ((analogRead(0)-white) / range) * 360;
    SerialUSB.println(analogRead(0));
    
    error = desiredAngle - currentAngle;
    float normalizedError = abs(error/360);
    float correctiveSpeed = normalizedError*maxSpeed;
    
    if (error >= 0){ 
      Dxl.goalSpeed(100, correctiveSpeed);
      delay(50);
    }
    else{
      Dxl.goalSpeed(100, int(correctiveSpeed) | 0x400);
      delay(50);
    }
  

  
}
