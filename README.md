# Sistem Absensi Karyawan dengan RFID Arduino Mega Komunikasi PLX DAQ dan Kontrol Ruangan Berbasis RFID, RTC, dan DHT11

Proyek ini merupakan sistem absensi otomatis berbasis RFID, yang dikembangkan menggunakan Arduino. Sistem ini tidak hanya mencatat kehadiran, tetapi juga mengontrol perangkat seperti lampu, kipas, dan solenoid, serta menampilkan data kehadiran dan suhu ruangan melalui LCD I2C. Sistem juga menggunakan RTC (Real-Time Clock) untuk mencatat waktu secara akurat dan sensor suhu DHT11 untuk mengontrol kipas berdasarkan suhu ruangan. Proyek ini merupakan implementasi sistem absensi berbasis **RFID** yang terintegrasi dengan **LCD I2C**, **sensor suhu DHT11**, **RTC DS3231**, serta **aktuator** (lampu, kipas, solenoid, dan LED indikator). Sistem dapat mendeteksi **kedatangan (Datang)** dan **kepulangan (Pulang)** pengguna sesuai dengan data RFID yang sudah terdaftar, sekaligus mengatur perangkat ruangan secara otomatis.[SKRIPSI Mahasiswa Elektro - UMPO]

---

## 🛠️ Komponen yang Digunakan
- Arduino (Uno / Mega / sejenisnya)
- Modul RFID **RC522**
- Modul RTC **DS3231**
- Sensor Suhu **DHT11**
- LCD **I2C 20x4**
- LED Hijau & Merah
- 2 Lampu (relay/output digital)
- 2 Kipas (relay/output digital)
- Solenoid door lock
- Buzzer (opsional)
- Kabel jumper & breadboard / PCB

---

## ⚡ Pinout
| Komponen | Pin Arduino |
|----------|-------------|
| RFID SDA | D9 |
| RFID RST | D8 |
| DHT11 Data | D2 |
| LED Hijau | D5 |
| LED Merah | D6 |
| Lampu 1 | D14 (A0) |
| Lampu 2 | D15 (A1) |
| Kipas 1 | D16 (A2) |
| Kipas 2 | D17 (A3) |
| Solenoid | D18 (A4) |
| LCD I2C | SDA/SCL (A4/A5 untuk Uno) |

---

## 📌 Fitur Utama
1. **Absensi RFID**
   - Mendeteksi kartu RFID terdaftar.
   - Menampilkan informasi **Nama, ID, dan Divisi** di LCD.
   - Mengirim data ke **Serial Monitor** dalam format tabel (bisa digunakan ke Excel Logger).

2. **Manajemen Kehadiran**
   - Status **Datang** → mencatat jam masuk.  
   - Status **Pulang** → mencatat jam keluar.  
   - Data absensi dikelola melalui array `ID_RFID`, `dataNama`, `dataDivisi`, dan `dataKode`.

3. **Kontrol Ruangan Otomatis**
   - Lampu & solenoid diaktifkan/dimatikan oleh **Admin**.  
   - Kipas menyala otomatis berdasarkan suhu:
     - Kipas 1 → ≥ 25°C
     - Kipas 2 → ≥ 27°C  

4. **Keamanan**
   - Kartu tidak terdaftar → akses ditolak, LED merah menyala.

---

## 📂 Struktur Data
- **ID_RFID[]** → daftar UID kartu RFID.
- **dataNama[]** → nama pemilik kartu.
- **dataDivisi[]** → divisi pemilik (Admin / Karyawan / Siswa).
- **dataKode[]** → kode kategori (Admin=1, Karyawan=2, Siswa=3).
- **Hadir[]** → status kehadiran (Datang / Pulang).

---

## 📊 Format Data Serial (Export Excel)
```text
CLEARDATA
LABEL, Tanggal, Nama, No ID, Divisi, Jam Masuk, Jam Keluar
ROW,SET, <Nomor>
DATA, <Tanggal>, <Nama>, <ID>, <Divisi>, <Jam Masuk>, <Jam Keluar>

CONTOH: DATA, 04/10/2025, YOSEP, BBD5352F, ADMIN, 07:15:32, 16:02:11
```

---

## Contacs us : 
* [Frendi RoboTech](https://www.instagram.com/frendi.co/)
* [Whatsapp : +6287888227410](https://wa.me/+6287888227410)
* [Email    : frendirobotech@gmail.com](https://mail.google.com/mail/u/0/?view=cm&tf=1&fs=1&to=frendirobotech@gmail.com) atau [Email    : frendix45@gmail.com](https://mail.google.com/mail/u/0/?view=cm&tf=1&fs=1&to=frendix45@gmail.com)

---

## 👨‍💻 Author
Dikembangkan oleh: Imam Sa'id Nurfrendi [Reog Robotic & Robotech Electronics]  
Lisensi: Open Source (MIT)
