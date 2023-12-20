/*
                    <<Medical Clinic System>>
    Author : 
    Last Update: 2 Dec 2023
    
    <Class Patient>
     - setButton_Patient(int btn)
     - receive()
     - LCD_realtime()
    <Class Doctor>
     - setButton_Doctor(int btn)
     - recovery()
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define MAX_SIZE 10


/*
                      LCD
    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
 00          [  E  R  I  L     T  E  A  M  ]
 01                      2  0  2  3
 ____________________________________________________
    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
 00          [  W  E  L  C  O  M  E  ]
 01 P  a  t  i  e  n  t     :     0  0
 ____________________________________________________

     00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
 00     I  N  S  I  D  E  :     T  A  K  E  N
 01     P  a  t  i  e  n  t  :     0  0
*/
class Patient
{
public:
  void setButton_Patient(int btn)
  {
    Serial.begin(9600);
    if(validate_btn(btn))
    {
      patient_btn = btn;
      pinMode(btn, INPUT);
      for(int i = 0; i < MAX_SIZE; i++)
      {
        array[i] = "empty";
        ticket[i] = 0;
      }
      LCD_intro();
      Serial.println("\t[Patient] : ACK");
    }
    else
      Serial.println("\t[ERROR]\n  Patient-btn Undefined");
  }

  void receive()
  {
    if(validate_btn(patient_btn))
    {
      if(Pindex < MAX_SIZE)
      {
        delay(50);
        if(digitalRead(patient_btn))
        {
          total_patients++;
          if(random(2)) // == 1
          {
            array[Pindex] = "Taken";
            ticket[Pindex] = 1;
            Serial.print(++Pindex);
            Serial.print(" ");
            Serial.println("[Patient arrived]");
            //delay(3000);
          }
          else // == 0
          {
            array[Pindex] = "Lab";
            ticket[Pindex] = 1;
            Serial.print(++Pindex);
            Serial.print(" ");
            Serial.println("[Patient arrived]");
            //delay(5000);
          }
          LCD_receive();
        }
      }
      else
        Serial.println("Clinic System is Full");
    }
    else
    {
      Serial.println("\t[ERROR]\n  Patient-btn Undefined");
      delay(4000);
    }
  }

  void LCD_realtime()
  {
      lcd.setCursor(1,0);
      lcd.print("INSIDE : ");
      lcd.setCursor(9,0);
      lcd.print("empty");
      lcd.setCursor(1,1);
      lcd.print("Patient: ");
      lcd.setCursor(10,1);
      lcd.print(Pindex);
  }

  //  Data
protected:
  int Pindex = 0, total_patients = 0, ticket[MAX_SIZE];
  String array[MAX_SIZE];
  int patient_btn = -1;
private:
  void LCD_intro()
  {
    lcd.begin(16, 2);
    lcd.setBacklight(1);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(" ERIL TEAM ");
    lcd.setCursor(7,1);
    lcd.print(2023);
    delay(1400);
  }


  void LCD_receive()
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print(" WELCOME ");
    lcd.setCursor(1,1);
    lcd.print("Patient: ");
    lcd.setCursor(10,1);
    lcd.print(total_patients);
    delay(2000);
  }

  bool validate_btn(int btn)
  {
    return (btn >= 2 && btn < 14);
  }
};

class Doctor : public Patient
{
public:
  void setButton_Doctor(int btn)
  {
    if(validate_btn(btn))
    {
      doctor_btn = btn;
      Serial.println("\t[Doctor] : ACK");
    }
    else
     Serial.println("\t[ERROR]\n  Doctor-btn Undefined");
  }
  void recovery()
  {
    if(isBreak >= 2)
    {
      breakTime(5);
      isBreak = 0;
    }
    if(validate_btn(doctor_btn))
    {
      delay(200);
      if(digitalRead(doctor_btn))
      {
        if(Pindex > 0)
        {
          if(array[Pindex-1] == "Taken")
          {
            Taken();

          }
          else if(array[Pindex-1] == "Lab")
          {
            Lab();

          }
          else
          {
            Serial.println("\t[ERROR]");
            Serial.print("-> '");
            Serial.print(array[Pindex]);
            Serial.println("'");
            delay(2000);
          }
          }
        else
          Serial.println("\t[NOTICE]\n  No Patients in the system");
      }
    }
    else
    {
      Serial.println("\t[ERROR]\n  Doctor-btn Undefined");
      delay(4000);
    }
  }

private:
  bool validate_btn(int btn)
  {
    return(btn >= 2 && btn < 14 && btn != patient_btn);
  }

  void Taken()
  {
    Serial.println("RecovType : [Taken]");
    LCD_recovery();
    for(int i = 0; i < 3; i++)
    {
      delay(1000);
      Serial.print("### ");
    }
    dec();
    num_of_taken++;
    isBreak++;
    profit+=30.0;
    Serial.println("\n\t[Done]");
  }

  void Lab()
  {
    Serial.println("RecovType : [Lab]");
    LCD_recovery();
    for(int i = 0; i < 5; i++)
    {
      delay(1000);
      Serial.print("### ");
    }
    dec();
    num_of_labs++;
    isBreak++;
    profit+=50.0;
    Serial.println("\n\t[Done]");
  }

  void dec()
  {
    for(int i = 0; i < MAX_SIZE-1; i++)
      array[i] = array[i+1];
    array[Pindex] = "empty";
    ticket[Pindex] = 0;
    Pindex--;
  }

  void LCD_recovery()
  {
      lcd.setCursor(1,0);
      lcd.print("INSIDE : ");
      lcd.setCursor(9,0);
      lcd.print(array[0]);
      lcd.setCursor(1,1);
      lcd.print("Patient: ");
      lcd.setCursor(10,1);
      lcd.print(total_patients - Pindex + 1);
  }

  void breakTime(int sec)
  {
    Serial.println("[Break Time]");
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("BREAK");
    lcd.setCursor(6,1);
    lcd.print("TIME");
    delay(sec*1000);
    Serial.println("\t[Done]");
  }
  int doctor_btn = -1;


  //      Data
protected:
  int num_of_labs = 0, num_of_taken = 0, isBreak = 0;
  float profit = 0;
};



//(Base)Class: Patient
Doctor person;
const int greenLEDPin=8;
const int redLEDPin=9;
const int emergencySwitchPin = 2; 
volatile bool emergencyFlag = false; // Flag for emergency case
//volatile bool doctorBreak = false; // Flag for doctor's break
volatile bool emergencyConfirmed = false; // Flag for emergency confirmation
unsigned long previousMillis = 0; // To track time for break
unsigned long emergencyStartMillis = 0; // To track time for emergency case
int patientCount = 1;
int patientStatus = 0; // 0 - Normal, 1 - Lab, 2 - On Hold

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  person.setButton_Patient(4);
  person.setButton_Doctor(3);
    pinMode(greenLEDPin,OUTPUT);
  pinMode(redLEDPin,OUTPUT);
  pinMode(emergencySwitchPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(emergencySwitchPin), emergencyInterrupt, CHANGE);

  Serial.begin(9600); // Initialize serial communication
  Serial3.begin(9600);
}

void loop ()
{
  unsigned long currentMillis = millis();
  person.LCD_realtime();
  person.receive();
  person.recovery();
    digitalWrite(greenLEDPin,HIGH);
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    if (message == "Emergency") {
      emergencyFlag = true;
      emergencyConfirmed = false;
    }
  }
   // Handle confirmed emergency case
  if (emergencyFlag && !emergencyConfirmed) {
    lcd.clear();
    lcd.print("Emergency Case");
    if (confirmEmergency()) {
      emergencyConfirmed = true;
      lcd.clear();
      lcd.print("On Hold: ");
      lcd.print(patientCount);
      patientStatus = 2;
      digitalWrite(greenLEDPin, LOW);
      flashRedLED(); // Function to flash red LED using PWM
    }
  }
  // Handle emergency treatment
  if (emergencyConfirmed) {
    if (currentMillis - emergencyStartMillis >= 40000) { // 40 seconds treatment time
      patientStatus = 0; // Return to previous status
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
      emergencyFlag = false;
      emergencyConfirmed = false;
    }
  }

  
}
void emergencyInterrupt() {
  // Interrupt triggered by the emergency switch
  emergencyFlag = true;
}

bool confirmEmergency() {
  Serial3.println("Emergency Detected. Confirm? (Y/N)");
  while (Serial3.available() <= 0); // Wait for user input
  char response = Serial3.read();
  return (response == 'Y' || response == 'y');
}



void flashRedLED() {
  // Function to flash the red LED using PWM
  for (int i = 0; i < 255; i++) {  // Increase brightness
    analogWrite(redLEDPin, i);
    delay(50); // Adjust the delay time for the desired flash speed
  }
  for (int i = 255; i > 0; i--) {  // Decrease brightness
    analogWrite(redLEDPin, i);
    delay(50); // Adjust the delay time for the desired flash speed
  }
  analogWrite(redLEDPin, 0); // Turn off the LED
}
