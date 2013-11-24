#define LOW_VOLTAGE_LIMIT 11000
#define EMERGENCY_VOLTAGE_LIMIT 9900

void batteryCtrl_init();
void batteryCtrl_update();
int voltage;
int lowVoltageState;
int emergencyVoltageState;

setup()
{
  Serial.begin(9600);
  
  batteryCtrl_init();
}

loop()
{
 batteryCtrl_update();
 Serial.print("voltage: ");
 Serial.println(voltage);
 
 Serial.print("lov voltage: ");
 
 delay(100); 
}



int 

void batteryCtrl_init()
{
  pinMode(0, INPUT);
  voltage=0;
  lowVoltageState=0;
  emergencyVoltageState=0;
  
}
void batteryCtrl_update()
{
  
}
