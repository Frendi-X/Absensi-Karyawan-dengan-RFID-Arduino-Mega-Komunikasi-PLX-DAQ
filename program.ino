#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "RTClib.h"
#include <Wire.h>

#define pinSDA 9
#define pinRST 8
#define DHTPIN 2
#define DHTTYPE DHT11
#define ledHijau 5
#define ledMerah 6
#define lampu1 14
#define lampu2 15
#define kipas1 16
#define kipas2 17
#define solenoid 18
#define ON true
#define OFF false
#define Datang 1
#define Pulang 2

LiquidCrystal_I2C lcd(0x27, 20 , 4);
MFRC522 RFID(pinSDA, pinRST);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;

byte index,
     cek,
     loopp = 1,
     loopp_tempC = 0,
     jml_admin = 0;
byte letter;
// float tempC;

const byte banyak_data = 100;

String Admin = "1",
       Karyawan = "2",
       Siswa = "3",
       lastAdmin = "",
       currentUidStr = "",
       content = "";

boolean ACC_Kartu = false,
        lampuMenyala = false;

char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
char buffer[70];

unsigned long hitungan_milis;
unsigned long milis_sekarang;
const unsigned long nilai = 1000;

String ID_RFID[banyak_data] = {"NULL",
                               "BBD5352F", "F9B7EC15", "09923316", "44AD0804", "14F10304",
                               "A4C30904", "64130404", "8450E003", "B400D603", "94E00004",
                               "E46F0A04", "55F8940A", "651E000A", "9436D803", "54F3E203",
                               "CBC8722F", "0488E303", "65E5220A", "CB07212F", "BBEDB42F",

                               "CBFD492F", "D9EB8B15", "F3F5E902", "A498E603", "098C8816",
                               "65A2EB0A", "BBB9732F", "65EB040A", "65A14D0A", "55F5350A",
                               "E9ECC115", "65F4B70A", "F4CC0404", "2410D303", "09C9C416",
                               "CB26702F", "E97ADA15", "F959B215", "0949AF16", "54B8EF03"
                              };

String dataNama[banyak_data] = {"NULL",
                                "YOSEP" , "MIKO", "LIKA", "AYU", "LINA",
                                "ERLYN", "MEGA", "RAHAYU", "DIAN", "EMMA",
                                "JULI", "RANI", "WIDI", "TIFFANY", "RATIH",
                                "STEVANI", "DIYARA", "WULAN", "EGA", "PUTRI",

                                "LAKEISHA", "ADHYASTHA", "DIKA", "NAURA", "SALSABELA",
                                "AYRELL", "AKHTAR", "RAMSHA", "JESSLYN", "AINA",
                                "ARSYILA", "JERICHO", "DAMAR", "JONATHAN", "FELISHINTA",
                                "ANGELINA", "JOYCELLA", "JEREMY", "FLORENCE", "MONICA"
                               };

String dataDivisi[banyak_data] = {"NULL",
                                  "ADMIN", "ADMIN", "MAT", "MAT", "MAT",
                                  "MAT", "MAT", "MAT", "MAT", "MAT",
                                  "MAT", "MAT", "MAT", "EFL", "EFL",
                                  "EFL", "EFL", "EFL", "EFL", "EFL",

                                  "SISWA", "SISWA", "SISWA", "SISWA", "SISWA",
                                  "SISWA", "SISWA", "SISWA", "SISWA", "SISWA",
                                  "SISWA", "SISWA", "SISWA", "SISWA", "SISWA",
                                  "SISWA", "SISWA", "SISWA", "SISWA", "SISWA"
                                 };

String dataKode[banyak_data] = {"NULL",                        // Data Kode (Admin = 1; Karyawan = 2; Siswa = 3)
                                "1" , "1", "2", "2", "2",
                                "2", "2", "2", "2", "2",
                                "2", "2", "2", "2", "2",
                                "2", "2", "2", "2", "2",

                                "3", "3", "3", "3", "3",
                                "3", "3", "3", "3", "3",
                                "3", "3", "3", "3", "3",
                                "3", "3", "3", "3", "3"
                               };

byte Hadir[] = {1,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,

                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
               };


void Tampilan_Awal() {
  loopp = 1;
  lcd.setCursor(0, 0);
  lcd.print("  Tap Kartu Anda!  ");
}

String uidToString(MFRC522::Uid* uid) {
  String uidStr = "";
  for (byte i = 0; i < uid->size; i++) {
    if (uid->uidByte[i] < 0x10) {
      uidStr += "0";
    }
    uidStr += String(uid->uidByte[i], HEX);
  }
  uidStr.toUpperCase();
  return uidStr;
}


void displayTime() {
  DateTime now = rtc.now();
  if (now.hour() < 10) {
    lcd.print("0");
    lcd.print(now.hour());
  } else {
    lcd.print(now.hour());
  }
  lcd.print(":");
  if (now.minute() < 10) {
    lcd.print("0");
    lcd.print(now.minute());
  } else {
    lcd.print(now.minute());
  }
  lcd.print(":");
  if (now.second() < 10) {
    lcd.print("0");
    lcd.print(now.second());
  } else {
    lcd.print(now.second());
  }
}

void sendData_Datang() {
  DateTime now = rtc.now();
  Serial.println("ROW,SET," + String(index + 1));
  Serial.print("DATA");
  Serial.print(",");
  Serial.print(String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()));
  Serial.print(",");
  Serial.print(String(dataNama[index]));
  Serial.print(",");
  Serial.print(String(ID_RFID[index]));
  Serial.print(",");
  Serial.print(String(dataDivisi[index]));
  Serial.print(",");
  Serial.println(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
}

void sendData_Pulang() {
  DateTime now = rtc.now();
  Serial.println("DATA,");
  Serial.println(String("") + "CELL,SET,F" + String(index + 1) + "," + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  RFID.PCD_Init();
  dht.begin();
  lcd.init();
  lcd.backlight();

  //  if (! rtc.begin()) {
  //    Serial.println("Couldn't find RTC");
  //    Serial.flush();
  //    abort();
  //  }
  //
  //  if (rtc.lostPower()) {
  //    Serial.println("RTC lost power, let's set the time!");
  //    // When time needs to be set on a new device, or after a power loss, the
  //    // following line sets the RTC to the date & time this sketch was compiled
  //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //    // This line sets the RTC with an explicit date & time, for example to set
  //    // January 21, 2014 at 3am you would call:
  //    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //  }

  pinMode(ledHijau, OUTPUT);
  pinMode(ledMerah, OUTPUT);
  pinMode(lampu1, OUTPUT);
  pinMode(lampu2, OUTPUT);
  pinMode(kipas1, OUTPUT);
  pinMode(kipas2, OUTPUT);
  pinMode(solenoid, OUTPUT);

  digitalWrite(lampu1, HIGH);
  digitalWrite(lampu2, HIGH);
  digitalWrite(kipas1, HIGH);
  digitalWrite(kipas2, HIGH);
  digitalWrite(solenoid, HIGH);

  Serial.println("CLEARDATA");
  Serial.println("LABEL,Tanggal,Nama,No ID, Devisi, Jam Masuk, Jam Keluar");

  hitungan_milis = 0;
}

void loop() {
  DateTime now = rtc.now();
  float tempC = dht.readTemperature();

  Tampilan_Awal();
  lcd.setCursor(6, 3);
  displayTime();

  if (lampuMenyala == ON) {
    lcd.setCursor(6, 2);
    lcd.print(tempC);
    lcd.print(" ");
    lcd.print("C");
  } else {
    lcd.setCursor(0, 2);
    lcd.print("                    ");
  }

  if (tempC >= 25 && lampuMenyala == ON) {
    // Serial.println("Kipas 1 Menyala");
    digitalWrite(kipas1, LOW);
  } else {
    // Serial.println("Kipas 1 Mati");
    digitalWrite(kipas1, HIGH);
  }

  if (tempC >= 27 && lampuMenyala == ON) {
    // Serial.println("Kipas 2 Menyala");
    digitalWrite(kipas2, LOW);
  } else {
    // Serial.println("Kipas 2 Mati");
    digitalWrite(kipas2, HIGH);
  }

  if ( ! RFID.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! RFID.PICC_ReadCardSerial()) {
    return;
  }

  currentUidStr = uidToString(&RFID.uid);
  Serial.print("\nID : ");
  Serial.print(currentUidStr);
  Serial.println();

  //  content.toUpperCase();
  RFID.PICC_HaltA();
  RFID.PCD_StopCrypto1();

  for (cek = 1 ; cek <= banyak_data; cek++)
  {
    if (currentUidStr == ID_RFID[cek]) {
      ACC_Kartu = true;
      index = cek;
    }
  }

  /* ---------- RFID Terdaftar pada Sistem ---------- */
  if (currentUidStr == ID_RFID[index])
  {
    Hadir[index] = Hadir[index] + 1;

    /* ---------- Status Datang ---------- */
    if (Hadir[index] == Datang) {
      while (loopp <= 1) {
        // Serial.println("Datang");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Akses Diterima  ");
        digitalWrite(ledHijau, HIGH);
        delay(2000);
        sendData_Datang();
        digitalWrite(ledHijau, LOW);
        lcd.clear();
        lcd.setCursor(0, 3);
        lcd.print("Datang : ");
        displayTime();

        if (dataKode[index] == Admin) {
          jml_admin++;
          lcd.setCursor(0, 0);
          lcd.print("Nama: ");
          lcd.print(dataNama[index]);
          lcd.setCursor(0, 1);
          lcd.print("Id  : ");
          lcd.print(ID_RFID[index]);
          lcd.setCursor(0, 2);
          lcd.print("Div : ");
          lcd.print(dataDivisi[index]);
          delay(5000);

          for (byte indexAdmin = 0; indexAdmin <= index; indexAdmin++) {
            if (currentUidStr == ID_RFID[indexAdmin]) {
              if (!lampuMenyala) {
                loopp_tempC = 1;
                lampuMenyala = true;
                digitalWrite(lampu1, LOW);
                delay(500);
                digitalWrite(lampu2, LOW);
                delay(500);
                digitalWrite(solenoid, LOW);
                delay(500);
                lastAdmin = ID_RFID[indexAdmin];
                // Serial.println("\nLampu dinyalakan oleh : " + String(dataNama[indexAdmin]) + "\t ID : " + String(lastAdmin));
              } else if (lastAdmin == ID_RFID[indexAdmin]) {
                // Matikan lampu jika admin mendeteksi saat lampu menyala dan admin terakhir yang menyalakannya
                loopp_tempC = 0;
                lampuMenyala = false;
                digitalWrite(lampu1, HIGH);
                delay(500);
                digitalWrite(lampu2, HIGH);
                delay(500);
                digitalWrite(solenoid, HIGH);
                delay(500);
                // Serial.println("\nLampu dimatikan oleh  : " + String(dataNama[indexAdmin]) + "\t ID : " + String(lastAdmin));
              } else {
                // Serial.println("\n" + dataNama[indexAdmin] + String(" Terdeteksi, tetapi lampu tetap menyala"));
              }
              currentUidStr = "";
              break; // Keluar dari loop setelah menemukan admin yang cocok
            }
          }
        }
        else if (dataKode[index] == Karyawan) {
          lcd.setCursor(0, 0);
          lcd.print("Nama: ");
          lcd.print(dataNama[index]);
          lcd.setCursor(0, 1);
          lcd.print("Id  : ");
          lcd.print(ID_RFID[index]);
          lcd.setCursor(0, 2);
          lcd.print("Div : ");
          lcd.print(dataDivisi[index]);
          delay(5000);
        }
        else if (dataKode[index] == Siswa) {
          lcd.setCursor(0, 0);
          lcd.print("Nama: ");
          lcd.print(dataNama[index]);
          lcd.setCursor(0, 1);
          lcd.print("Id  : ");
          lcd.print(ID_RFID[index]);
          lcd.setCursor(0, 2);
          lcd.print("Div : ");
          lcd.print(dataDivisi[index]);
          delay(5000);
        }
        loopp = 2;
        break;
      }
    }

    /* ---------- Status Pulang ---------- */
    else if (Hadir[index] == Pulang) {
      while (loopp <= 1) {
        // Serial.println("Pulang");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Akses Diterima  ");
        digitalWrite(ledHijau, HIGH);
        delay(2000);
        sendData_Pulang();
        digitalWrite(ledHijau, LOW);
        lcd.clear();
        lcd.setCursor(0, 3);
        lcd.print("Pulang : ");
        displayTime();

        if (dataKode[index] == Admin) {
          jml_admin--;
          lcd.setCursor(0, 0);
          lcd.print("Nama: ");
          lcd.print(dataNama[index]);
          lcd.setCursor(0, 1);
          lcd.print("Id  : ");
          lcd.print(ID_RFID[index]);
          lcd.setCursor(0, 2);
          lcd.print("Div : ");
          lcd.print(dataDivisi[index]);
          delay(5000);

          for (byte indexAdmin = 0; indexAdmin <= index; indexAdmin++) {
            if (currentUidStr == ID_RFID[indexAdmin]) {
              if (!lampuMenyala) {
                loopp_tempC = 1;
                lampuMenyala = true;
                digitalWrite(lampu1, LOW);
                delay(500);
                digitalWrite(lampu2, LOW);
                delay(500);
                digitalWrite(solenoid, LOW);
                delay(500);
                lastAdmin = ID_RFID[indexAdmin];
                // Serial.println("\nLampu dinyalakan oleh : " + String(dataNama[indexAdmin]) + "\t ID : " + String(lastAdmin));
              } else if (jml_admin == 0) {
                // Matikan lampu jika admin mendeteksi saat lampu menyala dan admin terakhir yang menyalakannya
                loopp_tempC = 0;
                lampuMenyala = false;
                digitalWrite(lampu1, HIGH);
                delay(500);
                digitalWrite(lampu2, HIGH);
                delay(500);
                digitalWrite(solenoid, HIGH);
                delay(500);
                // Serial.println("\nLampu dimatikan oleh  : " + String(dataNama[indexAdmin]) + "\t ID : " + String(lastAdmin));
              } else {
                // Serial.println("\n" + dataNama[indexAdmin] + String(" Terdeteksi, tetapi lampu tetap menyala"));
              }
              currentUidStr = "";
              break; // Keluar dari loop setelah menemukan admin yang cocok
            }
          }
        }
        else if (dataKode[index] == Karyawan) {
          lcd.setCursor(0, 0);
          lcd.print("Nama: ");
          lcd.print(dataNama[index]);
          lcd.setCursor(0, 1);
          lcd.print("Id  : ");
          lcd.print(ID_RFID[index]);
          lcd.setCursor(0, 2);
          lcd.print("Div : ");
          lcd.print(dataDivisi[index]);
          delay(5000);
        }
        else if (dataKode[index] == Siswa) {
          lcd.setCursor(0, 0);
          lcd.print("Nama: ");
          lcd.print(dataNama[index]);
          lcd.setCursor(0, 1);
          lcd.print("Id  : ");
          lcd.print(ID_RFID[index]);
          lcd.setCursor(0, 2);
          lcd.print("Div : ");
          lcd.print(dataDivisi[index]);
          delay(5000);
        }
        loopp = 2;
        break;
      }
    }

    if (Hadir[index] >= 2) {
      Hadir[index] = 0;
    }

    currentUidStr = "";
    lcd.clear();
  }

  /* ---------- RFID Tidak Terdaftar pada Sistem ---------- */
  else if (currentUidStr != ID_RFID[index]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Akses Ditolak   ");
    digitalWrite(ledMerah, HIGH);
    delay(2000);
    digitalWrite(ledMerah, LOW);
    currentUidStr = "";
    lcd.clear();
  }
}
