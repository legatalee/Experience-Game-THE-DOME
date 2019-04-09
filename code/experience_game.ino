#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_SSD1306.h>

#define RST_PIN 9
#define SS_PIN  10

MFRC522 mfrc(SS_PIN, RST_PIN);

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  pinMode(8, OUTPUT);

  Serial.begin(115200);
  SPI.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  mfrc.PCD_Init();
}

int energy = 50;
int o2 = 50;
int co2 = 50;
int h2o = 50;

bool state = 0;
bool soundCheck = false;
bool loopCheck = true;
bool warningCheck = false;
String lastuid = "";
bool check = true;
unsigned long prev_time = 0;
unsigned long prev_time2 = 0;
unsigned long prev_time3 = 0;
int buzzCnt = 0;
int buzzCnt2 = 0;
int speed = 1;

void loop() {
  if (1 <= energy && energy <= 30) {//나중에 15로 바꾸기
    soundCheck = true;
  } else {
    soundCheck = false;
    buzzCnt = 0;
  }
  if (energy <= 0) {
    gameEnd();
  }
  if (o2 <= 0 && co2 >= 100 && h2o <= 0) {
    speed = 4;
  }
  else if (o2 <= 0 && co2 >= 100) {
    speed = 3;
  }
  else if (h2o <= 0 && co2 >= 100) {
    speed = 3;
  }
  else if (o2 <= 0 && h2o <= 0) {
    speed = 3;
  }
  else if (o2 <= 0) {
    speed = 2;
  }
  else if (co2 >= 100) {
    speed = 2;
  }
  else if (h2o <= 0) {
    speed = 2;
  } else speed = 1;


  unsigned long current_time = millis();
  if (current_time - prev_time > 60000) {

    if (energy > 0) {
      energy -= speed;
    }
    //    if (o2 > 0) {
    //      o2 -= 1;
    //    }
    //    if (co2 > 0) {
    //      co2 -= 1;
    //    }
    //    if (h2o < 100) {
    //      h2o += 1;
    //    }
    prev_time = millis();
  }

  unsigned long current_time2 = millis();
  if (soundCheck == true) {
    if (current_time2 - prev_time2 > 200) {
      if (buzzCnt < 16) {
        digitalWrite(8, loopCheck);
        loopCheck = !loopCheck;
        buzzCnt++;
      }
      if (buzzCnt == 24) digitalWrite(8, LOW);
      prev_time2 = millis();
    }
  }

  unsigned long current_time3 = millis();
  if (warningCheck == true) {
    if (current_time3 - prev_time3 > 1000) {
      if (buzzCnt2 < 3) {
        digitalWrite(8, HIGH);
        buzzCnt2++;
      } else {
        warningCheck = false;
        digitalWrite(8, LOW);
      }
      prev_time3 = millis();
    }
  }

  mfrc.PICC_ReadCardSerial();
  if (( mfrc.PICC_IsNewCardPresent() || mfrc.PICC_ReadCardSerial()) && (mfrc.uid.uidByte[0] != 0)) {//쓰레기값 처리
    mfrc.PICC_ReadCardSerial();
    Serial.print("Card UID:");
    String uid = "";
    for (byte i = 0; i < 4; i++) {
      Serial.print(mfrc.uid.uidByte[i]);
      uid += mfrc.uid.uidByte[i];
    }
    Serial.println();

    if (lastuid == uid) {//이전 입력받은 데이터랑 비교
      check = false;//같으면 미행동 선언
    }
    if (check) {
      doSomething(uid);//행동
    }
    check = true;
    lastuid = uid;//이전 데이터로 옮김
    delay(50);
  }
  displayAll();
}

void doSomething(String uid) {
  buzzCnt2 = 0;
  if (uid == "157231127106") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "10914522696") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "109185214160") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "10931214160") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "61132241160") {
    o2 += 50;
    co2 += 15;
    warningCheck = true;
  }

  if (uid == "17387225160") {
    o2 += 10;
    warningCheck = true;
  }
  if (uid == "237120230160") {
    h2o += 10;
    warningCheck = true;
  }
  if (uid == "61117240160") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "23718225217") {
    energy += 20;
    warningCheck = true;
  }
  if (uid == "01311430116") {
    h2o += 40;
    warningCheck = true;
  }
  if (uid == "157191229160") {
    co2 += 20;
    warningCheck = true;
  }
  if (uid == "013136237160") {
    o2 += 20;
    h2o += 20;
    warningCheck = true;
  }
  if (uid == "6167234160") {
    h2o += 10;
    warningCheck = true;
  }





  if (uid == "10939191185") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "1897820996") {
    o2 += 10;
    energy += 10;
    warningCheck = true;
  }
  if (uid == "9323521196") {
    o2 += 20;
    co2 += 10;
    energy += 10;
    warningCheck = true;
  }
  if (uid == "25343227185") {
    o2 += 20;
    co2 += 10;
    energy += 10;
    warningCheck = true;
  }
  if (uid == "20516522196") {
    co2 += 10;
    warningCheck = true;
  }
  if (uid == "0133320296") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }






  if (uid == "12523253185") {
    o2 += 15;
    h2o += 15;
    co2 += 15;
    energy += 20;
    warningCheck = true;
  }
  if (uid == "157173239185") {
    energy += 10;
    warningCheck = true;
  }
  if (uid == "4522820496") {
    o2 -= 15;
    h2o -= 15;
    co2 -= 15;
    energy -= 15;
    warningCheck = true;
  }
  if (uid == "2051122797") {
    h2o += 20;
    co2 += 20;
    energy += 20;
    warningCheck = true;
  }
  if (uid == "173147223185") {
    o2 += 10;
    h2o += 10;
    co2 += 10;
    energy += 10;
    warningCheck = true;
  }
  if (uid == "93166105106") {
    h2o += 10;
    co2 += 10;
    energy += 10;
    warningCheck = true;
  }




  if (uid == "20596248185") {
    co2 += 10;
    energy += 10;
    warningCheck = true;
  }
  if (uid == "1891602297") {
    energy += 10;
    warningCheck = true;
  }
  if (uid == "109154237185") {
    o2 -= 5;
    warningCheck = true;
  }
  if (uid == "1255423196") {
    h2o -= 5;
    warningCheck = true;
  }
  if (uid == "0133821196") {
    co2 -= 5;
    warningCheck = true;
  }
  if (uid == "2920723296") {
    energy -= 5;
    warningCheck = true;
  }
  if (uid == "18915922196") {
    energy -= 10;
    warningCheck = true;
  }
  if (uid == "173201245185") {
    o2 += 50;
    h2o += 50;
    co2 += 50;
    energy += 50;
    warningCheck = true;
  }





  if (uid == "14162209160") {
    o2 += 40;
    h2o += 40;
    co2 += 40;
    energy += 40;
    warningCheck = true;
  }
  if (uid == "253254250185") {
    o2 += 30;
    h2o += 30;
    co2 += 30;
    energy += 40;
    warningCheck = true;
  }
  if (uid == "1253123096") {
    o2 += 50;
    h2o += 50;
    co2 += 50;
    energy += 50;
    warningCheck = true;
  }
}
void gameEnd() {
  digitalWrite(8, HIGH);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(49, 0);
  display.setTextSize(6);
  display.println("+");
  display.setCursor(19, 48);
  display.setTextSize(2);
  display.println("HOSPITAL");
  display.display();
  delay(5000);
  digitalWrite(8, LOW);
  while (1);
}
void displayAll() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(2, 10);
  display.println(pad(energy));
  display.setCursor(74, 10);
  display.println(pad(o2));
  display.setCursor(2, 43);
  display.println(pad(co2));
  display.setCursor(74, 43);
  display.println(pad(h2o));

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(11, 0);
  display.println(F("Energy"));
  display.setCursor(94, 0);
  display.println(F("O2"));
  display.setCursor(19, 33);
  display.println(F("Co2"));
  display.setCursor(91, 33);
  display.println(F("H2O"));
  display.display();
}
String pad(int n) {
  String final = "";
  if (n < 100) final += '0';
  if (n < 10) final += '0';
  final += n;
  return final;
}
