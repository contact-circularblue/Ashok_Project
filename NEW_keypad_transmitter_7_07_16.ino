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

  // Turn on the blacklight and print a message.
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
        Serial.println("ACK");
        //Serial.println("1");
        break;
      case 1:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Insert SD Card"));
        Serial.println("ACK");
        //Serial.println("1");
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("SD CARD FOUND"));
        Serial.println("ACK");
        //Serial.println("2");
        break;
      case 3:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Sim Unavailable"));
        lcd.setCursor(0, 1);
        lcd.print(F("1-Retry,2-Ignore"));
        Serial.println("ACK");
        //Serial.println("3");
        break;
      case 4:
        lcd.clear();
        lcd.begin();
        lcd.setCursor(0, 0);
        lcd.print(F("Insert sim and restart machine"));
        Serial.println("ACK");
        break;
      case 5:
        //Serial.println("CASE 5");
        //lcd.begin();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("No GSM Features"));
        Serial.println("ACK");
        break;
      case 6:
        //Serial.println("CASE 5");
        lcd.begin();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Set time"));
        lcd.setCursor(0, 1);
        lcd.print(F("in 24 hr format"));
        Serial.println("ACK");
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
        //Serial.println("CASE 5");
        lcd.begin();
        //lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Set time"));
        lcd.setCursor(0, 1);
        lcd.print(F("HH:MM"));
        lcd.setCursor(0, 1);
        lcd.cursor();
        lcd.blink();



        Serial.println("ACK");



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


            Serial.println("ACK");
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

            lcd.setCursor(i+3, 1);
            lcd.print(received);


            Serial.println("ACK");
            i++;
          }
        }
        //Serial.println("ACK");
        break;
      case 22:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Checking Ntwrk"));
        Serial.println("ACK");
        break;
      case 107:
        Serial.println("ACK");
        keys();
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("DEFAULT"));
        Serial.println("ACK");
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
