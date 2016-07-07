/*
Sensor data format in SD card: (each line 25 bytes (including line ending))
Code(1 byte),Name(6 bytes),Start time(5 bytes) {00:00},End time(5 bytes) {00:00},Voice message(1 byte),Phone no. set(1 byte)


The circuit:
 * PORT EXPANDER CONNECTIONS
 * PE-1 (for keypad) address = 0x20 // Connect pins 15-17 to GND
 * PE-2 (for RF) address = 0x27 // Connect pins 15-17 to VCC

 * LCD CONNECTIONS:
 * LCD RS pin to pin 15 (Ard. 9)
 * LCD Enable pin to pin 16 (Ard. 10)
 * LCD D4 pin to pin 11 (Ard. 5)
 * LCD D5 pin to pin 12 (Ard. 6)
 * LCD D6 pin to pin 13 (Ard. 7)
 * LCD D7 pin to pin 14 (Ard. 8)
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)


 * Keypad has 12 keys and is alpha-neumeric (3X4 keypad)
 * 1(abc)  2(def)  3(ghi)
 * 4(jkl)  5(mno)  6(pqr)
 * 7(stu)  8(vwx)  9(yz)
 *    *       0      #
 * where key * = integer 11 (For entering programming mode) and key # = integer 12

 * SD CARD CONNECTIONS
 * CS->Pin5 (Ard. 3)
 * SCK (CLOCK)->Pin19 (Ard. 13)
 * MISO->Pin18 (Ard. 12)
 * MOSI->Pin17 (Ard. 11)

*/



#include <SoftwareSerial.h>

#include <avr/pgmspace.h>
//#include <LiquidCrystal.h>
#include <string.h>
#include <SD.h>
#include <SPI.h>
#include <PortExpander_I2C.h>
#include "Wire.h"
#include "RTClib.h"
#include <EEPROM.h>

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

RTC_DS1307 rtc;

//LiquidCrystal lcd(9, 10, 5, 6, 7, 8);


bool CheckSDCard();
bool CheckSimCard();
bool CheckNumberSetSD();
bool SensorDBCheck();
bool ProgrammingModeAuthentication();
bool ProgModeKeyCheck();
bool isValidCheck(int DecVal);
void GetNumbersOnSD();
void LCDDisplayScrollLine1(char a[]);
void StoreSensorDB();
void ProgrammingMode();
void StoreSensorDB2();
void VoiceMessageRec();
void ProgAdd();
void ProgConv();
void ProgHyb();
void AddSensor(int DecVal, int edit, int mode);
void AddModeTriggerCheck();
void sendtoGSM(String Name, String tval, int ph_num, int voice);
void statusreturn();
void UploadToServer(String S_Name, String tval);
void statusreturn2();
//void EditSensor(int DecVal, int pos);
char DetectKeypress();
int ReadDecoder();
int ipow(int base, int exp);
int FindSensor();
bool CmpTimeGrtr(String(fstarttime), String tval);
bool CmpTimeSmlr(String(fstarttime), String tval);
void yn();
//void wait();
char keypress();

SoftwareSerial LCD_comm(5, 6);


//const char string_0[] PROGMEM = "Switch off the machin;e, insert the sim card and try again.            ";
//char buffer[70];

//const char* const string_table[] PROGMEM = {string_0};

bool SDCardInstalled = 0;
bool SimPresent = 0;
bool GSMFeature = 0;
bool NumberSetPresentOnSD = 0;
bool NumbersOnSD = 0;
bool SensorDBStored = 0;
bool AddressableModeActive = 1;
bool ConventionalModeActive = 0;
int play = A0;
int fwd = A1;
int rec = A2;
int rst = A3;


char Keypress;
char password[] = "6666";

byte serial_word = 0;


void setup() {

  LCD_comm.begin(9600);
  Serial.begin(9600);
  //Serial.println("READY");
  //lcd.printbegin(16, 2);

  pinMode(4, INPUT);
  pinMode(2, INPUT);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(play, HIGH);
  digitalWrite(fwd, HIGH);
  digitalWrite(rec, HIGH);
  digitalWrite(rst, HIGH);

  //strcpy_P(buffer, (char*)pgm_read_word(&(string_table[0])));
  //Serial.print(buffer);

  //char string[16]=;
  //Serial.print(60);
  //write_serially(0);
  //////lcd.printprint(F("Installing systm"));////////////////////com1
  /////////Uncomment this
  //delay(7000);





  Serial.println(("AT"));////////////////////com2
  //statusreturn();
  delay(500);

  SDCardInstalled = CheckSDCard();
  while (SDCardInstalled == 0)
  {
    //Serial.println("Sending");
    write_serially(1);
    //lcd.printclear();
    ////lcd.printprint(F("Insert SD Card"));////////////////////com3
    SDCardInstalled = CheckSDCard();
  }
  if (SDCardInstalled == 1)
  {
    write_serially(2);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("SD CARD FOUND"));////////////////////com4
    delay(2000);
    //while(1);
    SimPresent = CheckSimCard();


    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    if (SimPresent == 0)
    {
      write_serially(3);
      ////lcd.printprint(F("Sim Unavailable"));////////////////////com5
      //delay(1000);
      //lcd.printsetCursor(0,1);
      ////lcd.printprint(F("1-Retry,2-Ignore"));

      Keypress = 0;
      Serial.println("while");
      while ((Keypress != '1') && (Keypress != '2'))
      {
        write_serially(107);
        Keypress = DetectKeypress();
      }

      if (Keypress == '1')
      {
        write_serially(4);
        //lcd.printclear();
        ////lcd.printprint(F("Insert sim and restart machine"));////////////////////com7

        delay(2000);
        while (1)
        {
          for (int positionCounter = 0; positionCounter < 40; positionCounter++)
          {
            //lcd.printscrollDisplayLeft();
            delay(150);
          }
        }
      }

      if (Keypress == '2')
      {
        write_serially(5);
        //lcd.printclear();
        ////lcd.printprint(F("No GSM"));////////////////////com8

        Serial.println("MOVED");
        SimPresent = 0;
        GSMFeature = 0;
        Keypress = 0;
        delay(3000);
        write_serially(6);
        //lcd.printclear();
        ////lcd.printprint(F("Set time"));////////////////////com9
        //lcd.printsetCursor(0,1);
        ////lcd.printprint(F("in 24 hr format"));////////////////////com10
        delay(3000);
        //write_serially(7);
        ////lcd.printclear();
        //////lcd.printprint(F("Set date"));
        ////lcd.printsetCursor(0,1);
        //////lcd.printprint(F("YYYY/MM/DD"));
        ////lcd.printsetCursor(0,1);
        ////lcd.printcursor();
        ////lcd.printleftToRight();
        int i = 0;
        char a = 0;
        //int yy=0;
        //int mm=0;
        //int dd=0;
        int hh = 0;
        int mi = 0;
        /*
        for(i=3;i>-1;i--)
        {
          Keypress = DetectKeypress();
          yy=yy+(Keypress-48)*ipow(10,i);
          ////lcd.printprint(Keypress);
        }
        //lcd.printsetCursor(5,1);
        for(i=1;i>-1;i--)
        {
          Keypress = DetectKeypress();
          mm=mm+(Keypress-48)*ipow(10,i);
          ////lcd.printprint(Keypress);
        }
        //lcd.printsetCursor(8,1);
        for(i=1;i>-1;i--)
        {
          Keypress = DetectKeypress();
          dd=dd+(Keypress-48)*ipow(10,i);
          ////lcd.printprint(Keypress);

        }
        */
        delay(500);

        write_serially(8);
        //lcd.printclear();
        //////lcd.printprint(F("Set time"));
        //lcd.printsetCursor(0,1);
        ////lcd.printprint(F("HH:MM"));////////////////////com11
        //lcd.printsetCursor(0,1);
        //lcd.printcursor();
        //lcd.printleftToRight();
        //keypress=0;
        for (i = 1; i > -1; i--)
        {
          Keypress = 0;

          while (Keypress == 0)
          {
            Serial.println("Detecting Keypress");
            //write_serially(107);
            Keypress = DetectKeypress();
          }
          Serial.print("Keypress=");
          Serial.println(Keypress);
          hh = hh + (Keypress - 48) * ipow(10, i);
          LCD_comm.print(Keypress);
          delay(500);
          while (!LCD_comm.find("ACK"));
          Serial.println("ACK RECEIVED");
          ////lcd.printprint(Keypress);
        }

        //lcd.printsetCursor(3,1);
        for (i = 1; i > -1; i--)
        {
          Keypress = 0;

          while (Keypress == 0)
          {
            Serial.println("Detecting Keypress");
            //write_serially(107);
            Keypress = DetectKeypress();
          }
          Serial.print("Keypress=");
          Serial.println(Keypress);
          mi = mi + (Keypress - 48) * ipow(10, i);
          LCD_comm.print(Keypress);
          delay(500);
          while (!LCD_comm.find("ACK"));
          Serial.println("ACK RECEIVED");
        }
        if (! rtc.begin())
        {
          write_serially(11);
          //lcd.printclear();
          //lcd.printsetCursor(0,0);
          ////lcd.printprint(F("ERROR"));////////////////////com12
          while (1);
        }
        else
          rtc.adjust(DateTime(0, 0, 0, hh, mi, 0));


        write_serially(12);
        //lcd.printnoCursor();

        //lcd.printclear();
        //lcd.printsetCursor(0,1);
        ////lcd.printprint(F("System clock set"));////////////////////com13

      }
    }

    if (SimPresent == 1)
    {
      //delay(1000);
      write_serially(13);
      //lcd.printclear();
      //lcd.printsetCursor(0,0);
      ////lcd.printprint(F("SIM ready"));////////////////////com14
      delay(2000);
      NumberSetPresentOnSD = CheckNumberSetSD();
      if (NumberSetPresentOnSD == 0)
      {
        ////Serial.print("\nNumbers not found\n");
        write_serially(14);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("Nos. not found"));////////////////////com15
        delay(2000);
        GetNumbersOnSD();
        NumbersOnSD = 1;
      }

      else
      {
        NumbersOnSD = 1;
        write_serially(15);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("...Nos. found..."));////////////////////com16
        delay(2000);
      }
    }
  }
  write_serially(16);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Chekng Sensor DB"));////////////////////com17
  delay(2000);

  SensorDBStored = SensorDBCheck();


  if (SensorDBStored == 0)
  {
    write_serially(17);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Snsr DB nt found"));////////////////////com18
    delay(2000);
    StoreSensorDB();
    write_serially(18);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Progmng complete"));////////////////////com19
    delay(2000);
  }
  else
  {
    write_serially(19);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Snsr DB found"));////////////////////com20
    delay(2000);
    //StoreSensorDB();
  }
  write_serially(20);
  //lcd.printclear();
  //lcd.printsetCursor(3,1);
  ////lcd.printprint(F("System on"));////////////////////com21
  ////lcd.printsetCursor(0,0);
  //////lcd.printprint(F("Prs # fr prg mod"));
}

void loop()
{
  if (AddressableModeActive == 1 && ConventionalModeActive == 0) // Addressable Mode is Active
  {
    bool pkey = ProgModeKeyCheck ();
    if (pkey == 1)
    {
      ProgrammingMode();
    }

    AddModeTriggerCheck();

  }

  if (AddressableModeActive == 0 && ConventionalModeActive == 1) // Conventional Mode is Active
  {
    bool pkey = ProgModeKeyCheck ();
    if (pkey == 1)
    {
      ProgrammingMode();
    }

    //Serial.print("conv mode\n");
    CModeTriggerCheck();
  }

  if (AddressableModeActive == 1 && ConventionalModeActive == 1) // Hybrid Mode is Active
  {
    bool pkey = ProgModeKeyCheck ();
    if (pkey == 1)
    {
      ProgrammingMode();
    }
    AddressableModeActive = 1;
    ConventionalModeActive = 0;
    AddModeTriggerCheck();
    AddressableModeActive = 0;
    ConventionalModeActive = 1;
    CModeTriggerCheck();
    AddressableModeActive = 1;
    ConventionalModeActive = 1;
  }

}

void CModeTriggerCheck()
{
  int DecVal = 0;
  DecVal = FindSensor();
  if (DecVal != 0)
  {
    //Serial.print("DecVal= ");
    //Serial.println(DecVal);
    delay(2000);
    bool isValid = isValidCheck(DecVal);
  }
}


bool isValidCheck(int DecVal)
{
  int pos = 0;
  int i = 1;
  char SensorInfo[] = "0000000000000000000000";
  bool proceed = 0;
  //File file = SD.open("amode.txt", FILE_WRITE);
  File file;
  if (AddressableModeActive == 0 && ConventionalModeActive == 1)
    file = SD.open("cmode.txt", FILE_WRITE);

  if (AddressableModeActive == 1 && ConventionalModeActive == 0)
    file = SD.open("amode.txt", FILE_WRITE);

  //Serial.println("checking");

  pos = 0;
  //pos=pos+26;
  file.seek(pos);

  //DecVal='3';
  while (file.available() && proceed == 0)
  {
    //pos=file.position()-1;
    //file.seek(pos);
    //Serial.println(F("valid check"));

    //proceed=1;
    int check = 0;
    check = (file.read());

    //check=check-254;
    //Serial.print(F("from file, value= "));
    //Serial.println(check);
    //Serial.print(F("from sensor, value= "));
    //Serial.println(DecVal);

    delay(2000);

    /*
    Serial.print("from here: ");
    int val=0;

    while(val!=-1)
    {
    val=(file.read());
    Serial.print(char(val));
    if(val==DecVal)
    Serial.println("found");
    delay(500);
    }
    */

    if (check == DecVal)
    {
      //Serial.println("Sensor open");

      //bool TimeValid=TimeValidCheck();
      pos = pos + 2;
      file.seek(pos);
      for (i = 0; i < 22; i++)
      {
        SensorInfo[i] = file.read();
        //delay(500);
      }
      //Serial.println(SensorInfo);
      i = 0;
      String S_Name = "";

      for (i = 0; i < 6; i++)
        S_Name += SensorInfo[i];

      //Serial.println(S_Name);

      String S_Start = "";

      i = 7;
      for (i = 7; i < 9; i++)
        S_Start += SensorInfo[i];

      S_Start += ':';

      i = 10;
      for (i = 10; i < 12; i++)
        S_Start += SensorInfo[i];

      //Serial.println(S_Start);

      String S_Stop = "";

      i = 13;
      for (i = 13; i < 15; i++)
        S_Stop += SensorInfo[i];

      S_Stop += ':';

      i = 16;
      for (i = 16; i < 18; i++)
        S_Stop += SensorInfo[i];

      //Serial.println(S_Stop);

      int voice_msg = (SensorInfo[19] - 48);

      //Serial.println(voice_msg);

      int num_set = (SensorInfo[21] - 48);

      Serial.println(num_set);

      file.close();

      String  tval = "";


      //Serial.println(F("Get time"));


      if (SimPresent == 1)
      {
        // Serial.println(F("GSM time"));
        tval = getGSMtime();
        //while(1);
      }
      else
      {
        DateTime now = rtc.now();
        tval += (now.hour());
        tval += ':';
        tval += (now.minute());
        Serial.print(tval);
      }
      //Serial.println(S_Start);
      //Serial.println(S_Stop);

      bool ValidTrigger1 = CmpTimeGrtr(String(S_Start), tval);
      bool ValidTrigger2 = CmpTimeSmlr(String(S_Stop), tval);

      Serial.print(ValidTrigger1);
      Serial.print(" & ");////////////////////com22
      Serial.println(ValidTrigger2);

      if (ValidTrigger1 == 1 && ValidTrigger2 == 1)
      {
        write_serially(21);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("Sensor Triggered"));////////////////////com23

        //while(1);
        if (SimPresent == 1)
        {
          sendtoGSM(String(S_Name), String(tval), num_set, voice_msg);
        }

        Serial.println(F("Trigger is valid 'SIM'"));////////////////////com24
        String log_data = "";
        log_data += "Sensor name \"";
        log_data += S_Name;
        log_data += "\" ";
        log_data += "triggered at ";
        log_data += tval;
        log_data += "\n";

        //Serial.println(log_data);
        file.close();

        delay(1000);

        File dataFile;
        dataFile = SD.open("LOG.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.println(log_data);
          dataFile.close();
          // print to the serial port too:
          //Serial.println(log_data);
        }
        // if the file isn't open, pop up an error:
        else {
          //Serial.println(F("error opening datalog.txt"));
        }


        delay(500);
      }
      //while(1);
    }
    pos = pos + 26;
    file.seek(pos);

  }

  file.close();
}


void AddModeTriggerCheck()
{
  if (digitalRead(2) == HIGH)
  {
    int DecVal = ReadDecoder();
    bool isValid = isValidCheck(DecVal);
  }
}

bool ProgModeKeyCheck()
{
  //Serial.println("Prog key chck");
  if (digitalRead(4) == HIGH)
  {
    Keypress = DetectKeypress();
    //Serial.println(Keypress);
    if (Keypress == '#')
    {
      bool stat = 1;
      return stat;
    }
    else
    {
      bool stat = 0;
      return stat;
    }
  }
  bool stat = 0;
  return stat;

}

bool CheckSDCard()
{
  bool statusSD = 0;
  statusSD = SD.begin(3);
  return statusSD;
}

bool CheckNumberSetSD()
{
  bool ret = 0;
  int total_found = 0;

  if (SD.exists("1.txt"))
    total_found++;
  if (SD.exists("2.txt"))
    total_found++;
  if (SD.exists("3.txt"))
    total_found++;
  if (total_found == 3)
    ret = 1;
  return ret;
}



char DetectKeypress()
{
  PortExpander_I2C pe(0x20);
  pe.init();
  for ( int i = 0; i < 8; i++ )
  {
    pe.pinMode(i, INPUT);
  }

  byte k = '%';
  while (k == '%')
  {
    if (digitalRead(4) == HIGH)
    {
      while (digitalRead(4) == HIGH);
      Wire.requestFrom(32, 1);
      if (Wire.available())    //If the request is available
      {
        k = Wire.read();     //Receive the data
      }
    }
  }
  return k;
}



bool CheckSimCard()
{
  write_serially(22);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Checking Ntwrk"));////////////////////com25
  delay(3000);
  Serial.println(F("AT+CREG?"));////////////////////com26

  bool ret = 0;
  bool a = Serial.find("+CREG: 0,1");

  if (a == 0)
  {
    return ret;
  }

  else
  {
    ret = 1;
    return ret;
  }
}


void GetNumbersOnSD()
{ write_serially(23);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Enter Ph. numbrs"));////////////////////com27
  delay(200);
  //lcd.printsetCursor(0,1);
  ////lcd.printprint("11 digits each");////////////////////com28
  delay(2000);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);

  if (SD.exists("1.txt")) ////////////////////com29
    SD.remove("1.txt");////////////////////com30
  if (SD.exists("2.txt")) ////////////////////com31
    SD.remove("2.txt");////////////////////com32
  if (SD.exists("3.txt")) ////////////////////com33
    SD.remove("3.txt");////////////////////com34

  int num_set = 1;
  for (num_set = 1; num_set < 4; num_set++)
  {
    int iteration = 1;
    char num[60];
    File dataFile;
    write_serially(24);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    if (num_set == 1)
    {
      write_serially(25);
      ////lcd.printprint(F("Entr no. set 1"));////////////////////com35
    }
    if (num_set == 2)
    {
      write_serially(26);
      ////lcd.printprint(F("Entr no. set 2"));////////////////////com36
    }
    if (num_set == 3)
    {
      write_serially(27);
      ////lcd.printprint(F("Entr no. set 3"));////////////////////com37
    }
    write_serially(28);
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Please wait.."));////////////////////com38
    //wait();
    delay(2000);
    String dataString = "";
    char Keypress = '%';
    while (Keypress != '2')
    {
      delay(200);
      write_serially(29);
      //lcd.printclear();
      //lcd.printsetCursor(0,0);
      ////lcd.printprint (F("Entr ph number"));////////////////////com39
      delay(200);
      write_serially(30);
      Serial.print(iteration);
      //lcd.printsetCursor(14,0);
      ////lcd.printprint (iteration);
      iteration++;
      delay(2000);
      int i = 0;
      write_serially(31);
      ////lcd.printclear();
      //lcd.printsetCursor(0,1);
      //lcd.printcursor();
      //lcd.printleftToRight();
      while (i < 11)
      {
        ////////uncomment this
        Keypress = DetectKeypress();
        i++;
        write_serially(32);
        Serial.print(Keypress);
        ////lcd.printprint(Keypress);
        //////uncomment this
        dataString += String(Keypress);

        //////comment this
        //dataString += '9';
      }
      dataString += ",";
      write_serially(33);
      //lcd.printclear();
      //lcd.printsetCursor(0,0);
      ////lcd.printprint(F("Enter more nos.?"));////////////////////com40
      //lcd.printsetCursor(0,1);
      ////lcd.printprint(F("Yes-1, No-2"));////////////////////com41
      ////uncomment this
      Keypress = DetectKeypress();



    }
    if (num_set == 1)
    {
      dataFile = SD.open("1.txt", FILE_WRITE);
    }
    if (num_set == 2)
    {
      dataFile = SD.open("2.txt", FILE_WRITE);
    }
    if (num_set == 3)
    {
      dataFile = SD.open("3.txt", FILE_WRITE);
    }
    dataFile.println(dataString);
    dataFile.close();
    //Serial.print("\nSaving....\n");
  }
  delay(500);
  write_serially(34);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint (F("No. set complete"));////////////////////com42
  delay(2000);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint (F("Saving numbers.."));////////////////////com43
  delay(2000);
  //lcd.printnoCursor();

}

bool SensorDBCheck()
{
  bool ret = 0;
  if (SD.exists("amode.txt") && SD.exists("cmode.txt"))
  {
    ret = 1;
    return ret;
  }
  return ret;
}

void StoreSensorDB()
{
  //lcd.printclear();
  Keypress = 0;
  write_serially(35);
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Entr Prog Mode"));////////////////////com44
  //lcd.printsetCursor(0,1);
  ////lcd.printprint(F("Press #"));////////////////////com45
  while (Keypress != '#')  //// Detect the press of # key
  {
    /////////////// uncomment this
    Keypress = DetectKeypress();
    ///////////// remove this
    //Keypress='#';
  }
  delay(200);
  write_serially(36);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Entrng Prog Mode"));////////////////////com46
  ////lcd.printsetCursor(0,1);
  //////lcd.printprint(F("Please wait.."));////////////////////com47
  delay(2000);
  ////wait();
  ProgrammingMode();
}

void ProgrammingMode()
{
  bool i = 0;
  /////////////// uncomment this
  i = ProgrammingModeAuthentication();


  if (i == 0)
  {
    write_serially(37);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Wrong Password"));////////////////////com48
    //lcd.printsetCursor(0,1);
    ////lcd.printprint(F("Restart Machine"));////////////////////com49
    while (1);
  }

  if (i == 1)
  {
    write_serially(38);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Password Acceptd"));////////////////////com50
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Please wait.."));////////////////////com51
    //wait();
    delay(2000);

    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Enter Ph. Nos.?"));////////////////////com52
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Yes-1  No-2"));////////////////////com53
    yn();
    Keypress = 0;

    //while(Keypress!='1' & Keypress!='2')
    // {
    //Keypress=DetectKeypress();
    //}
    Keypress = keypress();
    if (Keypress == '1')
      GetNumbersOnSD();
    write_serially(39);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Enter snsr data?"));////////////////////com54
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Yes-1  No-2"));////////////////////com55
    yn();

    Keypress = 0;

    // while(Keypress!='1' & Keypress!='2')
    // {
    // Keypress=DetectKeypress();
    // }
    Keypress = keypress();
    if (Keypress == '1')
      StoreSensorDB2();
    write_serially(40);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Rec voice msgs?"));////////////////////com56
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Yes-1  No-2"));////////////////////com57
    yn();

    Keypress = 0;
    //while(Keypress!='1' & Keypress!='2')
    // {
    // Keypress=DetectKeypress();
    // }
    Keypress = keypress();
    if (Keypress == '1')
      VoiceMessageRec();
    write_serially(41);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Select devc mode"));////////////////////com58
    //lcd.printsetCursor(0,1);
    ////lcd.printprint(F("Cnv-1 Ad-2 Hyb-3"));////////////////////com59

    Keypress = 0;
    while (Keypress != '1' && Keypress != '2' && Keypress != '3')
    {
      Keypress = DetectKeypress();
    }

    if (Keypress == '1')
    {
      ConventionalModeActive = 1;
      AddressableModeActive = 0;
    }

    if (Keypress == '2')
    {
      AddressableModeActive = 1;
      ConventionalModeActive = 0;
    }

    if (Keypress == '3')
    {
      AddressableModeActive = 1;
      ConventionalModeActive = 1;
    }
    write_serially(42);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Progrming Complt"));////////////////////com60
    delay(2000);

    //lcd.printclear();
    //lcd.printsetCursor(3,1);
    ////lcd.printprint("System ON");
  }
}

bool ProgrammingModeAuthentication()
{
  int i = 0;
  bool ret = 0;
  write_serially(43);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Entr 4 dgt Pswd"));////////////////////com61

  //lcd.printsetCursor(0,1);
  //lcd.printcursor();
  //lcd.printleftToRight();
  for (i = 0; i < 4; i++)
  {
    //Keypress='6';
    ///uncomment this
    Keypress = DetectKeypress();
    if (Keypress != password[i])
    {
      write_serially(44);
      //lcd.printnoCursor();
      return ret;
    }
    write_serially(45);
    //lcd.printsetCursor(i,1);
    ////lcd.printprint("*");
  }
  write_serially(46);
  //lcd.printnoCursor();
  ret = 1;
  return ret;
}

void StoreSensorDB2()
{
  write_serially(47);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Prog sensors?"));
  ////lcd.printsetCursor(0,1);
  //////lcd.printprint(F("Yes-1  No-2"));
  yn();

  Keypress = 0;
  //while(Keypress!='1' & Keypress!='2')
  // {
  // Keypress=DetectKeypress();
  // }
  Keypress = keypress();
  if (Keypress == '1')
  {
    write_serially(48);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Select mode"));
    //lcd.printsetCursor(0,1);
    ////lcd.printprint(F("Ad-1 Cnv-2"));

    Keypress = 0;
    while (Keypress != '1' & Keypress != '2' & Keypress != '3')
    {
      Keypress = DetectKeypress();
    }

    if (Keypress == '1')
      ProgAdd();
    if (Keypress == '2')
      ProgConv();
    if (Keypress == '3')
      ProgHyb();
  }
}

void VoiceMessageRec()
{
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Prs & hold ERASE"));
  delay(10000);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Release ERASE"));
  write_serially(49);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Record 3 msgs of"));
  //lcd.printsetCursor(0,1);
  ////lcd.printprint(F("6 seconds each"));
  delay(2000);

  digitalWrite(rst, LOW);
  delay(200);
  digitalWrite(rst, HIGH);
  delay(200);


  int i = 1;
  for (i = 1; i < 4; i++)
  {
    write_serially(50);
    Serial.print(i);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Recrd msg "));
    ////lcd.printprint(i);
    delay(1000);
    //lcd.printsetCursor(0,1);
    ////lcd.printprint(F("Recording..."));
    digitalWrite(rec, LOW);
    delay(6000);
    digitalWrite(rec, HIGH);
    delay(500);
    write_serially(51);
    Serial.print(i);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Msg "));
    ////lcd.printprint(i);
    ////lcd.printprint(F(" recorded"));
    delay(2000);
  }
  write_serially(52);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Rec complete!"));

}

void ProgAdd()
{
  int pos = 0;
  int i = 1;
  int DecVal = 0;
  bool proceed = 0;
  while (proceed == 0)
  {
add_sensor:
    write_serially(53);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Init snsr additn"));
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Please wait....."));
    //wait();
    delay(2000);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Prss snsr to add"));
    while (digitalRead(2) == LOW);
    DecVal = ReadDecoder();
    write_serially(54);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Release Sensor"));
    pinMode(2, INPUT);
    while (digitalRead(2) == HIGH);

    File file = SD.open("amode.txt", FILE_WRITE);

    pos = 0;

    while (file.seek(pos) && proceed == 0)
    {
      byte check = 0;
      check = (file.read());
      if (check == DecVal)
      {
        pos = (file.position() - 1);
        write_serially(55);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("Snsr alrdy regtd"));
        //lcd.printsetCursor(0,1);
        ////lcd.printprint(F("Edit?Yes-1  No-2"));
        Keypress = 0;
        while (Keypress != '1' && Keypress != '2')
        {
          Keypress = DetectKeypress();
        }

        if (Keypress == '1')
        {
          AddSensor(DecVal, 1, 0);  // Mode=0 for addressable mode
          write_serially(56);
          //lcd.printclear();
          //lcd.printsetCursor(0,0);
          ////lcd.printprint(F("Sensr edit cmplt"));
          delay(2000);
        }
        write_serially(57);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("Add anthr sensr?"));
        ////lcd.printsetCursor(0,1);
        //////lcd.printprint(F("Yes-1  No-2"));
        yn();

        Keypress = 0;
        //while(Keypress!='1' & Keypress!='2')
        //{
        //Keypress=DetectKeypress();
        //}
        Keypress = keypress();

        if (Keypress == '1')
          goto add_sensor;
        if (Keypress == '2')
        {
          proceed = 1;
        }
      }
      pos = pos + 26;

    }

    file.close();


    if (proceed == 0)
    {
      AddSensor(DecVal, 0, 0); // Mode=0 for addressable mode
      write_serially(58);
      //lcd.printclear();
      //lcd.printsetCursor(0,0);
      ////lcd.printprint(F("Add anthr sensr?"));
      ////lcd.printsetCursor(0,1);
      //////lcd.printprint(F("Yes-1  No-2"));
      yn();

      Keypress = 0;
      //while(Keypress!='1' & Keypress!='2')
      //{
      //Keypress=DetectKeypress();
      //}
      Keypress = keypress();

      if (Keypress == '2')
        proceed = 1;
    }

  }

}

int ReadDecoder()
{
  //Serial.println("DECODER");
  PortExpander_I2C pe(0x27);
  pe.init();
  for ( int i = 0; i < 8; i++ )
  {
    pe.pinMode(i, INPUT);
  }
  byte k = 0;
  int val = 0;
  int i = 0;

  for (i = 0; i < 4; i++)
  {
    int b = pe.digitalRead(i);
    if (b == 1)
    {
      val = val + ipow(2, i);
    }
  }
  //Serial.println(val);
  return val;
}

int ipow(int base, int exp)
{
  int result = 1;
  while (exp)
  {
    if (exp & 1)
      result *= base;
    exp >>= 1;
    base *= base;
  }

  return result;
}

void AddSensor(int DecVal, int edit, int mode)
{
  File file;
  if (mode == 0)
  {
    //Serial.println("Amode");
    file = SD.open("amode.txt", FILE_WRITE);
  }
  if (mode == 1)
  {
    //Serial.println("Cmode");
    file = SD.open("cmode.txt", FILE_WRITE);
  }

  if (edit == 0)
  {
    //int pos=file.position();

    //file.seek(pos-1);
  }
  if (edit == 1)
  {
    //char a=0;
    file.seek(0);

    while (DecVal != file.read());



    file.seek(file.position() - 1);
  }

  char value = DecVal;
  file.print(value);
  file.print(",");

  String dataString = "";
  int i = 0;

  write_serially(59);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Enter 6 Dgt Name"));

  //lcd.printsetCursor(0,1);
  //lcd.printcursor();
  //lcd.printleftToRight();
  while (i < 6)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(60);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  dataString += ",";
  file.print(dataString);
  delay(500);
  write_serially(61);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Enter start time"));
  //lcd.printsetCursor(0,1);
  //lcd.printcursor();
  //lcd.printleftToRight();
  i = 0;

  dataString = "";
  while (i < 2)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(62);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  write_serially(63);
  ////lcd.printprint(F(":"));
  dataString += ':';
  file.print(dataString);
  i = 0;

  dataString = "";
  while (i < 2)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(64);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  dataString += ',';
  file.print(dataString);

  delay(500);
  write_serially(65);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Enter stop time"));
  //lcd.printsetCursor(0,1);
  //lcd.printcursor();
  //lcd.printleftToRight();
  i = 0;

  dataString = "";
  while (i < 2)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(66);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  write_serially(67);
  ////lcd.printprint(F(":"));
  dataString += ':';
  file.print(dataString);
  i = 0;

  dataString = "";
  while (i < 2)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(68);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  dataString += ",";
  file.print(dataString);

  delay(500);
  write_serially(69);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Voice msg 1,2or3"));
  //lcd.printsetCursor(0,1);
  //lcd.printcursor();
  //lcd.printleftToRight();
  i = 0;

  dataString = "";
  while (i < 1)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(70);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  dataString += ",";
  file.print(dataString);

  delay(500);
  write_serially(71);
  //lcd.printclear();
  //lcd.printsetCursor(0,0);
  ////lcd.printprint(F("Ph no set 1,2or3"));
  //lcd.printsetCursor(0,1);
  //lcd.printcursor();
  //lcd.printleftToRight();
  i = 0;

  dataString = "";
  while (i < 1)
  {
    Keypress = DetectKeypress();
    i++;
    write_serially(72);
    ////lcd.printprint(Keypress);
    dataString += String(Keypress);
  }
  file.println(dataString);
  write_serially(73);
  //lcd.printnoCursor();

  file.close();
  //Serial.println("Saved");
}





bool CmpTimeGrtr(String(fstarttime), String tval)
{
  //Serial.print("Start time: ");
  //Serial.println(fstarttime);
  //Serial.print("current time: ");
  //Serial.println(tval);

  if (fstarttime[0] < tval[0])
  {
    //Serial.print("current time greater\n");
    return 1;
  }
  else if (fstarttime[0] > tval[0])
  {
    //Serial.print("current time lesser\n");
    return 0;
  }
  else if (fstarttime[1] == tval[1])
  {
    if (fstarttime[1] < tval[1])
    {
      //Serial.print("current time greater\n");
      return 1;
    }
    else if (fstarttime[1] > tval[1])
    {
      //Serial.print("current time lesser\n");
      return 0;
    }
    else if (fstarttime[1] == tval[1])
    {
      if (fstarttime[3] < tval[3])
      {
        //Serial.print("current time greater\n");
        return 1;
      }
      else if (fstarttime[3] > tval[3])
      {
        //Serial.print("current time lesser\n");
        return 0;
      }
      else if (fstarttime[3] == tval[3])
      {
        if (fstarttime[4] < tval[4])
        {
          //Serial.print("current time greater\n");
          return 1;
        }
        else if (fstarttime[4] > tval[4])
        {
          //Serial.print("current time lesser\n");
          return 0;
        }
        else if (fstarttime[4] == tval[4])
        {
          //Serial.print("both times are equal\n");
          return 1;
        }
      }
    }
  }
}




bool CmpTimeSmlr(String(fstoptime), String tval)
{
  String fstarttime = fstoptime;
  //Serial.print("Stop time: ");
  //Serial.println(fstarttime);
  //Serial.print("current time: ");
  //Serial.println(tval);
  if (fstarttime[0] < tval[0])
  {
    //Serial.print("current time greater\n");
    //Serial.print("1\n");
    return 0;
  }
  else if (fstarttime[0] > tval[0])
  {
    //Serial.print("current time lesser\n");
    //Serial.print("2\n");
    return 1;
  }
  else if (fstarttime[1] == tval[1])
  {
    if (fstarttime[1] < tval[1])
    {
      //Serial.print("current time greater\n");
      //Serial.print("3\n");
      return 0;
    }
    else if (fstarttime[1] > tval[1])
    {
      //Serial.print("current time lesser\n");
      //Serial.print("4\n");
      return 1;
    }
    else if (fstarttime[1] == tval[1])
    {
      if (fstarttime[3] < tval[3])
      {
        //Serial.print("current time greater\n");
        //Serial.print("5\n");
        return 0;
      }
      else if (fstarttime[3] > tval[3])
      {
        //Serial.print("current time lesser\n");
        //Serial.print("6\n");
        return 1;
      }
      else if (fstarttime[3] == tval[3])
      {
        if (fstarttime[4] < tval[4])
        {
          //Serial.print("current time greater\n");
          //Serial.print("7\n");
          return 0;
        }
        else if (fstarttime[4] > tval[4])
        {
          //Serial.print("current time lesser\n");
          //Serial.print("8\n");
          return 1;
        }
        else if (fstarttime[4] == tval[4])
        {
          //Serial.print("both times are equal\n");
          //Serial.print("9\n");
          return 1;
        }
      }
    }
  }
}

void sendtoGSM(String Name, String tval, int ph_num, int voice)
{
  //Serial.println(tval);
  //while(1);
  File file;//=SD.open("DUMMY.txt", FILE_READ);
  if (ph_num == 1)
  {
    file = SD.open("1.txt", FILE_READ);
  }
  if (ph_num == 2)
  {
    file = SD.open("2.txt", FILE_READ);
  }
  if (ph_num == 3)
  {
    file = SD.open("3.txt", FILE_READ);
  }

  file.seek(0);
  int number = 0;
  int i = 0;
  int done = 0;
  char num[] = "00000000000";
  for (number = 0; number < 5; number++)
  {
    for (i = 0; i < 11; i++)
    {
      num[i] = (file.read());
    }
    int pos = file.position();
    file.seek(pos + 1);
    //Serial.print(F("Send SMS to "));
    //Serial.println(num);
    //while(1);

    Serial.println(F("AT"));
    statusreturn();
    delay(500);

    //// send SMS
    Serial.print(F("AT+CMGF=1\n"));         // AT command to send SMS message
    statusreturn();
    delay(200);

    String send_cmd = "";
    send_cmd += "AT+CMGS=\"";
    send_cmd += num;
    send_cmd += "\"";

    String msg = "";
    msg += "Sensor name \"";
    msg += Name;
    msg += "\" triggered at ";
    msg += tval;
    msg += ".";

    Serial.println(send_cmd);
    delay(200);
    Serial.println(msg);
    delay(200);
    Serial.write(0X1A);
    //Serial.println((char)26);
    delay(200);
    Serial.println();
    delay(10000);


    Serial.println(F("AT+CLCC=0"));
    statusreturn();
    Serial.println(F("AT+CLCC=1"));
    statusreturn();


    String call = "";
    call += "ATD";
    call += num;
    call += ";";

    Serial.println(call);

    statusreturn();

    delay(200);

    delay(4000);
    bool calling = 0;
    calling = (Serial.find(("1,0,3,0,0")));
    //Serial.print(calling);
    if (calling == 1)
    {
      //Serial.println(F("Ringing"));
      delay(500);
      Serial.println(F("AT+CLCC=0"));
      statusreturn();
      Serial.println(F("AT+CLCC=1"));
      statusreturn();
      while (1)
      {
        bool active = 0;
        active = Serial.find("1,0,0,0,0");
        bool busy = 0;
        busy = Serial.find("1,0,6,0,0");
        if (active == 1)
        {

          digitalWrite(rst, LOW);
          delay(200);
          digitalWrite(rst, HIGH);
          delay(300);
          i = 0;
          //int till=
          for (i = 0; i < (voice - 1); i++)
          {
            digitalWrite(fwd, LOW);
            delay(200);
            digitalWrite(fwd, HIGH);
            delay(500);
          }
          for (i = 0; i < 3; i++)
          {
            digitalWrite(play, LOW);
            delay(500);
            digitalWrite(play, HIGH);
            delay(6000);
          }
          break;
        }
        else if (busy == 1)
        {
          //Serial.println("Busy");
          break;
        }
      }
    }
    Serial.println(F("ATH"));
    delay(1000);
    Serial.flush();
    delay(500);
    UploadToServer(Name, tval);

  }
  //while(1);
}


void UploadToServer(String S_Name, String tval)
{
  Serial.println(F("AT"));
  statusreturn();
  Serial.println(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
  statusreturn();
  Serial.println(F("AT+SAPBR=3,1,\"APN\",\"www\""));
  statusreturn();
  Serial.println(F("AT+SAPBR =1,1"));
  delay(4000);
  Serial.println(F("AT+HTTPINIT"));
  delay(1000);
  Serial.println(F("AT+HTTPPARA=\"CID\",1"));
  statusreturn();
  Serial.println(F("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update\""));
  statusreturn();
  Serial.println(F("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\""));
  statusreturn();
  Serial.println(F("AT+HTTPDATA=47,10000"));
  delay(1000);
  Serial.print(F("key=DQLALFCLNBIFIXL7&field1="));
  Serial.print((tval));
  Serial.print(F("&field2="));
  Serial.print((S_Name));
  statusreturn();
  Serial.println(F("AT+HTTPACTION=1"));
  statusreturn();

}

void statusreturn()
{
  delay(500);
  bool a = Serial.find("OK");
  if (a == 1)
  {
    //Serial.println("OK");
  }
  else
  {
    //Serial.println("ERROR");
    write_serially(74);
    ////lcd.printclear();
    ////lcd.printsetCursor(0,0);
    //////lcd.printprint(F("GSM ERROR"));
    //delay(5000);
  }
}

void statusreturn2()
{
  delay(500);
  bool a = Serial.find("+HTTPACTION:1,200,1");
  if (a == 1)
  {
    //Serial.println("OK");
  }
  else
  {
    //Serial.println("ERROR");
    write_serially(75);
    ////lcd.printclear();
    ////lcd.printsetCursor(0,0);
    //////lcd.printprint(F("GSM ERROR"));
    //delay(5000);
  }
}



void ProgConv()
{
  //Serial.println("prog conv");
  int pos = 0;
  int i = 1;
  int DecVal = 0;
  int Sensor = 0;
  bool proceed = 0;
  bool proceed2 = 0;
  while (proceed2 == 0)
  {
    write_serially(76);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Init snsr additn"));
    ////lcd.printsetCursor(0,1);
    //////lcd.printprint(F("Please wait....."));
    //wait();
    delay(2000);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Prss snsr to add"));
    while (Sensor == 0)
      Sensor = FindSensor();
    DecVal = Sensor;
    //Serial.print("Value=");
    //Serial.println(Sensor);
    //while(1);
    write_serially(77);
    //lcd.printclear();
    //lcd.printsetCursor(0,0);
    ////lcd.printprint(F("Release Sensor"));
    while (Sensor != 0)
      Sensor = FindSensor();

    proceed = 0;

    File file;
    file = SD.open("cmode.txt", FILE_WRITE);

    pos = 0;
    file.seek(pos);

    while (file.available() && proceed == 0)
    {
      int check = 0;
      check = (file.read());

      ////Serial.print("DecVal= ");
      //Serial.println(DecVal);

      //Serial.print("Value from file= ");
      //Serial.println(check);

      if (check == DecVal)
      {
        pos = (file.position() - 1);
        write_serially(78);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("Snsr alrdy regtd"));
        //lcd.printsetCursor(0,1);
        ////lcd.printprint(F("Edit?Yes-1  No-2"));
        Keypress = 0;
        while (Keypress != '1' && Keypress != '2')
        {
          Keypress = DetectKeypress();
        }

        if (Keypress == '1')
        {
          AddSensor(DecVal, 1, 1);  // Mode=1 for conv mode
          write_serially(79);
          //lcd.printclear();
          //lcd.printsetCursor(0,0);
          ////lcd.printprint(F("Sensr edit cmplt"));
          delay(2000);
        }
        write_serially(80);
        //lcd.printclear();
        //lcd.printsetCursor(0,0);
        ////lcd.printprint(F("Add anthr sensr?"));
        ////lcd.printsetCursor(0,1);
        //////lcd.printprint(F("Yes-1  No-2"));
        yn();
        Keypress = 0;
        // while(Keypress!='1' & Keypress!='2')
        // {
        // Keypress=DetectKeypress();
        // }
        Keypress = keypress();
        if (Keypress == '1')
        {
          proceed = 1;
          file.close();
        }
        if (Keypress == '2')
        {
          proceed = 1;
          proceed2 = 1;
          file.close();
        }
      }
      pos = pos + 26;
      file.seek(pos);

    }

    // file.close();


    if (proceed == 0)
    {
      AddSensor(DecVal, 0, 1); // Mode=1 for conv mode
      file.close();
      write_serially(81);
      //lcd.printclear();
      //lcd.printsetCursor(0,0);
      ////lcd.printprint(F("Add sensr?"));
      ////lcd.printsetCursor(0,1);
      //////lcd.printprint(F("Yes-1  No-2"));
      yn();
      Keypress = 0;
      //while(Keypress!='1' & Keypress!='2')
      //{
      //Keypress=DetectKeypress();
      //}
      Keypress = keypress();

      if (Keypress == '1')
        proceed = 0;
      if (Keypress == '2')
        proceed = 1;
      proceed2 = 1;
    }


  }
}

int FindSensor()
{

  PortExpander_I2C pe(0x23);
  pe.init();
  int i = 0;
  int a = 0;
  byte k = 0;
  byte c = 0;


  for ( int i = 0; i < 8; i++ ) {
    pe.pinMode(i, INPUT);
  }
  k = 0;
  Wire.requestFrom(35, 1);
  if (Wire.available())    //If the request is available
  {
    k = Wire.read();     //Receive the data
  }
  byte mask = B00000001;
  for (int j = 0; j < 8; j++)
  {

    c = k & mask;
    if (c == 0)
      return (j + 1);
    mask = mask << 1;
  }
  /*
   for(i=0;i<8;i++)
   {
     Serial.print("0X23=");
     Serial.println(i);
   a=pe.digitalRead(i);
   if(a==LOW)
   {
     return i+1;
   }
   delay(1000);
   }
   */
  PortExpander_I2C pe2(0x26);
  pe2.init();
  i = 0;
  for ( int i = 0; i < 8; i++ ) {
    pe2.pinMode(i, INPUT);
  }
  k = 0;
  Wire.requestFrom(38, 1);
  if (Wire.available())    //If the request is available
  {
    k = Wire.read();     //Receive the data
  }
  mask = B00000001;
  for (int j = 0; j < 8; j++)
  {

    c = k & mask;
    // Serial.print("mask=");
    // Serial.println(mask);
    //if(k=='1')
    // Serial.print("Value after AND");
    //Serial.print("=");
    //Serial.println(c);
    if (c == 0)
      return (j + 9);
    mask = mask << 1;

    // delay(1000);
  }
  /*
   for(i=0;i<8;i++)
   {
     Serial.print("0X26=");
     Serial.println(i);
   a=pe2.digitalRead(i);
   if(a==LOW)
   {
     return i+9;
   }
   delay(1000);
   }
   */
  if (c != 0)
    return 0;
}


String getGSMtime()
{
  /*
  while(Serial.available() > 0)
  {
    Serial.read();
  }
  Serial.end();
  Serial.begin(9600);
  */

  delay(1000);

  Serial.println("AT");
  statusreturn();

  Serial.println("AT+CCLK?");
  delay(500);
  ////Serial.println("Called ATDxxxxxxxxxx");
  // print response over serial port
  int n = 0;
  int ret = 0;
  bool a = Serial.find("+CCLK:");
  if (a == 0)
  {
    //Serial.println("!");
    //return ret;
  }

  else
  {
    char time[] = "00000";
    a = Serial.find(",");
    if (a == 1)
    {
      int i = 0;
      for (i = 0; i < 5; i++)
      {
        time[i] = Serial.read();
      }
      //Serial.println(time);
      return time;
    }
    //Serial.println("+CCLK: found\n");
    //while(1);

  }
}

void ProgHyb()
{

}
void yn()
{
  write_serially(82);
  //lcd.printsetCursor(0,1);
  ////lcd.printprint(F("Yes-1  No-2"));
}

char keypress()
{
  while (Keypress != '1' & Keypress != '2')
  {
    Keypress = DetectKeypress();
  }
  return Keypress;
}

void write_serially(byte a)
{
  Serial.println(a);
  LCD_comm.write(a);

  while (!LCD_comm.available());
  while (!LCD_comm.find("ACK"));
  Serial.print("ACK found");



  //while(LCD_comm.available())
  //LCD_comm.read();


}

