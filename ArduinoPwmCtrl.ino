#define PWR_KEY_DELAY 1000
#define ADC_DELAY  1000
#define PERIOD  6250

const int buttonPin = 6;                 // 按鈕(pushbutton)
const int pwmPin = 11; 
const int ledPin = 13;
const int powerPin = 10; 

boolean isOn= false;

unsigned long  adc_delay;
unsigned long  pwr_key_delay;
unsigned long  pwm_period;

int pwm_low_period;


void setup()
{
  Serial.begin(9600);
  init_gpio();
  get_adc();
  init_timer();
}

void init_gpio ()
{
  pinMode(A0, INPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);  
  pinMode(ledPin, OUTPUT);    
  pinMode(buttonPin, INPUT);        
}

void get_adc()
{
  int adc = analogRead(0);  
  pwm_low_period = map(adc, 0, 1023, PERIOD/10, PERIOD);     
  adc_delay = millis();
}

void init_timer()
{
  //adc_delay = micros();
  pwr_key_delay = millis();
  pwm_period = micros();    
}


void loop()
{  
  if(duringmCheck(adc_delay) > PWR_KEY_DELAY)
  {
   get_keystatus();
  }
  
  if(duringmCheck(adc_delay) > ADC_DELAY)
  {
      get_adc();            
      led_display();
  }
  
  if (isOn == true)
  {
      if(checkPowerPin() == LOW)
      {
           digitalWrite(powerPin,HIGH);
           Serial.print("power H!!\n");
           delay(500);
      }
      
      if(checkPwmPin() == HIGH)
      {
          if(duringCheck(pwm_period)> pwm_low_period)
          {
              digitalWrite(pwmPin,LOW);
              pwm_period = micros();
          }    
      }
      else
      {
         if(duringCheck(pwm_period)> (PERIOD - pwm_low_period))
         {
              digitalWrite(pwmPin,HIGH);
              pwm_period = micros();
         }
      }
  }else{
      digitalWrite(pwmPin,LOW);
      if(checkPowerPin() == HIGH)
      {
           delay(500);
           digitalWrite(powerPin,LOW);
           Serial.print("power L!!\n");
      }      
  }
  
}

void get_keystatus()
{
   if(checkButtonPin() == HIGH)
   {
       isOn = true;       
       pwr_key_delay = millis();
       Serial.print("high\n");
   }else{
       isOn = false;       
       pwr_key_delay = millis();     
       Serial.print("low\n");
   }
}

void led_display()
{
    digitalWrite(ledPin,isOn);   // led display
}

unsigned long duringCheck(unsigned long t)
{
    return abs(micros()-t);
}

unsigned long duringmCheck(unsigned long t)
{
    return abs(millis()-t);
}

boolean checkPwmPin()
{
  return digitalRead(pwmPin);
}

boolean checkPowerPin()
{
  return digitalRead(powerPin);
}

boolean checkButtonPin()
{
  return digitalRead(buttonPin);
}
