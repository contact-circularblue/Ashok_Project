//#include< LiquidCrystal.h>
//LiquidCrystal lcd(13,12,11,10,9,8);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


const int r1 = A3;
const int r2 = A2;
const int r3 = A1;
const int r4 = A0;
const int c1 = 13;
const int c2 = 12;
const int c3 = 11;

char alphkeypad();
void put_out(char data);
void setup()
{
  Serial.begin(9600);
  // initialize the LCD
  lcd.begin();

  // Turn on the backlight and print a message.
  lcd.backlight();
  lcd.print(F("Welcome"));
  for (int t = 0; t < 3; t++)
  {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }
  Serial.begin(9600);
  Serial.print("LED blinkin done\n");
  //////lcd.begin(16 , 2);
  ////lcd.clear();
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);

  pinMode(2, OUTPUT);
  pinMode(A5, OUTPUT);

  int i = 3;
  for (i = 3; i < 11; i++)
    pinMode(i, OUTPUT);
  digitalWrite(i, LOW);
}
void loop()
{

  int i = 0;

  if (Serial.available())
  {
    int a = Serial.read();
    Serial.print("Case received = ");
    Serial.println(a);

    switch (a)
    {
      case 0:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Installing systm"));
        Serial.println(F("ACK"));
        //Serial.println("1");
        break;
      case 1:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Insert SD Card"));
        Serial.println(F("ACK"));
        //Serial.println("1");
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("SD CARD FOUND"));
        Serial.println(F("ACK"));
        //Serial.println("2");
        break;
      case 3:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Sim Unavailable"));
        lcd.setCursor(0, 1);
        lcd.print(F("1-Retry,2-Ignore"));
        Serial.println(F("ACK"));
        //Serial.println("3");
        break;
      case 4:
        lcd.clear();
        lcd.begin();
        lcd.setCursor(0, 0);
        lcd.print(F("Insert sim and"));
        lcd.setCursor(0, 1);
        lcd.print(F("restart machine"));
        Serial.println(F("ACK"));
        break;
      case 5:
        //Serial.println("CASE 5");
        //lcd.begin();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("No GSM Features"));
        Serial.println(F("ACK"));
        break;
      case 6:
        //Serial.println("CASE 5");
        lcd.begin();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Set time"));
        lcd.setCursor(0, 1);
        lcd.print(F("in 24 hr format"));
        Serial.println(F("ACK"));
        break;
      /*
        case 8:
        lcd.begin();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Set time"));
        lcd.setCursor(0, 1);
        lcd.print(F("HH:MM"));

        Serial.println("ACK");
        break;
      */
      case 8:

        lcd.begin();
        //lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Set time"));
        lcd.setCursor(0, 1);
        lcd.print(F("HH:MM"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.blink();
        Serial.println(F("ACK"));

        //lcd.begin();
        i = 0;
        while (i < 2)
        {

          keys();
          //lcd.print(F("THIS"));
          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);


            Serial.println(F("ACK"));
            i++;
          }
        }
        lcd.setCursor(3, 1);
        i = 0;
        while (i < 2)
        {
          keys();
          //lcd.print(F("THIS"));
          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i + 3, 1);
            lcd.print(received);


            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 11:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("ERROR"));
        Serial.println(F("ACK"));
        break;

      case 12:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("System clock set"));
        Serial.println(F("ACK"));
        break;

      case 13:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("SIM ready"));
        Serial.println(F("ACK"));
        break;

      case 14:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Nos. not found"));
        Serial.println(F("ACK"));
        break;

      case 15:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("...Nos. found..."));
        Serial.println(F("ACK"));
        break;

      case 16:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Chekng Sensor DB"));
        Serial.println(F("ACK"));
        break;

      case 17:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Snsr DB nt found"));
        Serial.println(F("ACK"));
        break;

      case 18:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Progmng complete"));
        Serial.println(F("ACK"));
        break;

      case 19:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Snsr DB found"));
        Serial.println(F("ACK"));
        break;

      case 20:
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print(F("System on"));
        Serial.println(F("ACK"));
        break;

      case 21:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Sensor Triggered"));
        Serial.println(F("ACK"));
        break;

      case 22:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Checking Ntwrk"));
        Serial.println(F("ACK"));
        break;

      case 23:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter Ph. numbrs"));
        delay(1000);
        Serial.println(F("ACK"));
        lcd.setCursor(0, 1);
        lcd.print(F("11 digits each"));
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        Serial.println(F("ACK"));
        break;

      case 24:
        lcd.clear();
        lcd.setCursor(0, 0);
        Serial.println(F("ACK"));
        break;

      case 25:
        lcd.print(F("Entr no. set 1"));
        delay(1000);
        Serial.println(F("ACK"));
        break;

      case 26:
        lcd.print(F("Entr no. set 2"));
        delay(1000);
        Serial.println(F("ACK"));
        break;

      case 27:
        lcd.print(F("Entr no. set 3"));
        delay(1000);
        Serial.println(F("ACK"));
        break;

      case 28:
        lcd.setCursor(0, 1);
        lcd.print(F("Please wait.."));
        Serial.println(F("ACK"));
        break;

      case 29:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Entr ph number"));
        Serial.println(F("ACK"));
        break;

      case 30:
        lcd.setCursor(14, 0);
        Serial.println(F("ACK"));

        Serial.println("Receiving");
        delay(500);
        if (Serial.available())
        {
          char received = Serial.read();
          lcd.print(received);
          Serial.println(F("ACK"));
        }
        break;

      case 31:
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        i = 0;
        while (i < 11)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;
      /*
            case 32:
              lcd.print(F("Keypress"));
              Serial.println(F("ACK"));
              break;
      */
      case 33:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter more nos.?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1, No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 34:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("No. set complete"));
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Saving numbers.."));
        delay(2000);
        lcd.noCursor();
        Serial.println(F("ACK"));
        break;

      case 35:
        lcd.setCursor(0, 0);
        lcd.print(F("Entr Prog Mode"));
        lcd.setCursor(0, 1);
        lcd.print(F("Press #"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 36:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Entr Prog Mode"));
        lcd.setCursor(0, 1);
        lcd.print(F("Please wait.."));
        Serial.println(F("ACK"));
        break;

      case 37:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Wrong Password"));
        lcd.setCursor(0, 1);
        lcd.print(F("Restart Machine"));
        Serial.println(F("ACK"));
        break;

      case 38:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Password Acceptd"));
        lcd.setCursor(0, 1);
        lcd.print(F("Please wait.."));

        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter Ph. Nos.?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 39:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter snsr data?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 40:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Rec voice msgs?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 41:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Select devc mode"));
        lcd.setCursor(0, 1);
        lcd.print(F("Cnv-1 Ad-2 Hyb-3"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 42:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Progrming Complt"));
        delay(2000);
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print(F("System ON"));
        Serial.println(F("ACK"));
        break;


      case 43:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Entr 4 dgt Pswd"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        // lcd.blink();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        //lcd.begin();
        i = 0;
        while (i < 4)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();
            if (received == 'E')
            {
              Serial.println(F("ACK"));
              break;
            }

            lcd.setCursor(i, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;


      case 46:
        lcd.noCursor();
        Serial.println(F("ACK"));
        break;

      case 47:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Prog sensors?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 48:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Select mode"));
        lcd.setCursor(0, 1);
        lcd.print(F("Ad-1 Cnv-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 49:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Prs & hold ERASE"));
        delay(10000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Release ERASE"));

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Record 3 msgs of"));
        lcd.setCursor(0, 1);
        lcd.print(F("6 seconds each"));
        Serial.println(F("ACK"));
        break;

      case 50:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Recrd msg "));
        Serial.println(F("ACK"));

        Serial.println("Receiving");
        delay(500);
        if (Serial.available())
        {
          char received = Serial.read();
          lcd.print(received);
        }

        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print(F("Recording..."));
        Serial.println(F("ACK"));
        break;

      case 51:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Msg "));
        Serial.println(F("ACK"));

        Serial.println("Receiving");
        delay(500);
        if (Serial.available())
        {
          char received = Serial.read();
          lcd.print(received);
        }

        lcd.print(F(" recorded"));
        delay(2000);
        Serial.println(F("ACK"));
        break;


      case 52:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Rec complete!"));
        Serial.println(F("ACK"));
        break;

      case 53:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Init snsr additn"));
        lcd.setCursor(0, 1);
        lcd.print(F("Please wait....."));
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Prss snsr to add"));
        Serial.println(F("ACK"));
        break;

      case 54:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Release Sensor"));
        Serial.println(F("ACK"));
        break;

      case 55:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Snsr alrdy regtd"));
        lcd.setCursor(0, 1);
        lcd.print(F("Edit?Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 56:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Sensr edit cmplt"));
        Serial.println(F("ACK"));
        break;

      case 57:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Add anthr sensr?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 58:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Add anthr sensr?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 59:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter 6 Dgt Name"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        i = 0;
        while (i < 6)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 61:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter start time"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        i = 0;
        while (i < 2)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 63:
        lcd.setCursor(2, 1);
        lcd.print(F(":"));
        Serial.println(F("ACK"));

        lcd.setCursor(3, 1);
        i = 0;
        while (i < 2)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i + 3, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 65:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Enter stop time"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        i = 0;
        while (i < 2)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 67:
        lcd.setCursor(2, 1);
        lcd.print(F(":"));
        Serial.println(F("ACK"));

        lcd.setCursor(3, 1);
        i = 0;
        while (i < 2)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i + 3, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 69:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Voice msg 1,2or3"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        i = 0;
        while (i < 1)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 71:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Ph no set 1,2or3"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.leftToRight();
        Serial.println(F("ACK"));

        i = 0;
        while (i < 1)
        {
          keys();

          Serial.println("Receiving");
          delay(500);
          if (Serial.available())
          {
            char received = Serial.read();

            lcd.setCursor(i, 1);
            lcd.print(received);
            delay(1000);

            Serial.println(F("ACK"));
            i++;
          }
        }

        break;

      case 73:
        lcd.noCursor();
        Serial.println(F("ACK"));
        break;

      case 74:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("GSM ERROR"));
        delay(5000);
        Serial.println(F("ACK"));
        break;

      case 75:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("GSM ERROR"));
        delay(5000);
        Serial.println(F("ACK"));
        break;

      case 76:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Init snsr additn"));
        lcd.setCursor(0, 1);
        lcd.print(F("Please wait....."));
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Prss snsr to add"));
        Serial.println(F("ACK"));
        break;

      case 77:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Release Sensor"));
        Serial.println(F("ACK"));
        break;

      case 78:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Snsr alrdy regtd"));
        lcd.setCursor(0, 1);
        lcd.print(F("Edit?Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 79:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Sensr edit cmplt"));
        Serial.println(F("ACK"));
        break;

      case 80:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Add anthr sensr?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 81:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Add sensr?"));
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        keys();
        break;

      case 82:
        lcd.setCursor(0, 1);
        lcd.print(F("Yes-1  No-2"));
        Serial.println(F("ACK"));
        break;

      case 107:
        Serial.println(F("ACK"));
        keys();
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("DEFAULT"));
        Serial.println(F("ACK"));
        break;
    }
  }
}

void keys()
{
  char k = 0;
  k = alphkeypad();

  put_out(k);
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);
  Serial.print(k);

}

char alphkeypad() {

  char k = '@';
  char a = 'a';
  while (a != 'c') {
    digitalWrite(c1, HIGH); digitalWrite(c2, HIGH); digitalWrite(c3, HIGH);
    digitalWrite(r1, LOW); digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH); digitalWrite(r4, HIGH);
    if (digitalRead(c1) == LOW) {
      //lcd.clear();
      k = '1';
      Serial.print('1');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);

      if (digitalRead(c1) == LOW) {
        //lcd.clear();
        k = 'a';
        Serial.print('a');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);

        if (digitalRead(c1) == LOW) {
          //lcd.clear();
          k = 'b';
          Serial.print('b');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);

          if (digitalRead(c1) == LOW) {
            //lcd.clear();
            k = 'c';
            Serial.print('c');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
            //while(1);
          }
        }
      }
      a = 'c';
      return k;
      //while(1);

    }


    digitalWrite(r1, LOW); digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH); digitalWrite(r4, HIGH);
    if (digitalRead(c2) == LOW) {
      //lcd.clear();
      k = '2';
      Serial.print('2');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c2) == LOW) {
        //lcd.clear();
        k = 'd';
        Serial.print('d');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c2) == LOW) {
          //lcd.clear();
          k = 'e';
          Serial.print('e');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c2) == LOW) {
            //lcd.clear();
            k = 'f';
            Serial.print('f');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, LOW); digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH); digitalWrite(r4, HIGH);
    if (digitalRead(c3) == LOW) {
      //lcd.clear();
      k = '3';
      Serial.print('3');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c3) == LOW) {
        //lcd.clear();
        k = 'g';
        Serial.print('g');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c3) == LOW) {
          //lcd.clear();
          k = 'h';
          Serial.print('h');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c3) == LOW) {
            //lcd.clear();
            k = 'i';
            Serial.print('i');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, LOW);
    digitalWrite(r3, HIGH); digitalWrite(r4, HIGH);
    if (digitalRead(c1) == LOW) {
      //lcd.clear();
      k = '4';
      Serial.print('4');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c1) == LOW) {
        //lcd.clear();
        k = 'j';
        Serial.print('j');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c1) == LOW) {
          //lcd.clear();
          k = 'k';
          Serial.print('k');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c1) == LOW) {
            //lcd.clear();
            k = 'l';
            Serial.print('l');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, LOW);
    digitalWrite(r3, HIGH); digitalWrite(r4, HIGH);
    if (digitalRead(c2) == LOW) {
      //lcd.clear();
      k = '5';
      Serial.print('5');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c2) == LOW) {
        //lcd.clear();
        k = 'm';
        Serial.print('m');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c2) == LOW) {
          //lcd.clear();
          k = 'n';
          Serial.print('n');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c2) == LOW) {
            //lcd.clear();
            k = 'o';
            Serial.print('o');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, LOW);
    digitalWrite(r3, HIGH); digitalWrite(r4, HIGH);
    if (digitalRead(c3) == LOW) {
      //lcd.clear();
      k = '6';
      Serial.print('6');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c3) == LOW) {
        //lcd.clear();
        k = 'p';
        Serial.print('p');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c3) == LOW) {
          //lcd.clear();
          k = 'q';
          Serial.print('q');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c3) == LOW) {
            //lcd.clear();
            k = 'r';
            Serial.print('r');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, HIGH);
    digitalWrite(r3, LOW); digitalWrite(r4, HIGH);
    if (digitalRead(c1) == LOW) {
      //lcd.clear();
      k = '7';
      Serial.print('7');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c1) == LOW) {
        //lcd.clear();
        k = 's';
        Serial.print('s');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c1) == LOW) {
          //lcd.clear();
          k = 't';
          Serial.print('t');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c1) == LOW) {
            //lcd.clear();
            k = 'u';
            Serial.print('u');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c' ;
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, HIGH);
    digitalWrite(r3, LOW); digitalWrite(r4, HIGH);
    if (digitalRead(c2) == LOW) {
      //lcd.clear();
      k = '8';
      Serial.print('8');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c2) == LOW) {
        //lcd.clear();
        k = 'v';
        Serial.print('v');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c2) == LOW) {
          //lcd.clear();
          k = 'w';
          Serial.print('w');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
          if (digitalRead(c2) == LOW) {
            //lcd.clear();
            k = 'x';
            Serial.print('x');
            //digitalWrite(A5, HIGH);
            delay(200);
            //digitalWrite(A5, LOW);
            delay(500);
          }
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, HIGH);
    digitalWrite(r3, LOW); digitalWrite(r4, HIGH);
    if (digitalRead(c3) == LOW) {
      //lcd.clear();
      k = '9';
      Serial.print('9');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);
      if (digitalRead(c3) == LOW) {
        //lcd.clear();
        k = 'y';
        Serial.print('y');
        //digitalWrite(A5, HIGH);
        delay(200);
        //digitalWrite(A5, LOW);
        delay(500);
        if (digitalRead(c3) == LOW) {
          //lcd.clear();
          k = 'z';
          Serial.print('z');
          //digitalWrite(A5, HIGH);
          delay(200);
          //digitalWrite(A5, LOW);
          delay(500);
        }
      }
      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH); digitalWrite(r4, LOW);
    if (digitalRead(c1) == LOW) {
      //lcd.clear();
      k = '*';
      Serial.print('*');
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);

      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH); digitalWrite(r4, LOW);
    if (digitalRead(c2) == LOW) {
      //lcd.clear();
      k = '0';
      Serial.print('0');
      //digitalWrite(A5, HIGH);
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);


      a = 'c';
      return k;
    }
    digitalWrite(r1, HIGH); digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH); digitalWrite(r4, LOW);
    if (digitalRead(c3) == LOW) {
      //lcd.clear();
      k = '#';
      Serial.print('#');
      //digitalWrite(A5, HIGH);
      //digitalWrite(A5, HIGH);
      delay(200);
      //digitalWrite(A5, LOW);
      delay(500);

      a = 'c';
      return k;
    }

  }//WHILE ENDING
}//KEYPAD ENDING


void put_out(char indata)
{
  byte data = indata;
  byte mask = 00000001;
  int go = 0;
  int i = 10;


  while (go < 8)
  {
    if (data & mask)
    {
      digitalWrite(i, HIGH);
    }
    else
    {
      digitalWrite(i, LOW);
    }

    i--;
    mask <<= 1;
    go++;
  }
}
