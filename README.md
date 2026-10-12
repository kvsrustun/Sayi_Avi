# 🎯 Deneyap ile İki Oyunculu Sayı Avı ve Elektronik Zar Oyunu

<p align="center">
  <img src="https://img.shields.io/badge/Donanım-Deneyap%20Kart%20(ESP32)-007acc?style=for-the-badge&logo=espressif&logoColor=white" alt="Donanım" />
  <img src="https://img.shields.io/badge/Ekran-2x%20Deneyap%20OLED%20(I2C)-8b5cf6?style=for-the-badge" alt="OLED" />
  <img src="https://img.shields.io/badge/Matris-2x%205x7%20Kırmızı%20LED-ef4444?style=for-the-badge" alt="LED Matris" />
  <img src="https://img.shields.io/badge/Kontrol-2x%20Analog%20Kumanda%20Kolu-10b981?style=for-the-badge" alt="Joystick" />
  <img src="https://img.shields.io/badge/Protokol-I2C%20Çoklu%20Veri%20Yolu-f59e0b?style=for-the-badge" alt="I2C" />
  <img src="https://img.shields.io/badge/Lisans-MIT-brightgreen?style=for-the-badge" alt="Lisans" />
</p>

> **Retro arcade ve refleks odaklı donanım projesi:** Deneyap Kart ekosistemi kullanılarak geliştirilen, iki oyuncunun bağımsız OLED ekranlar ve joystick kumandalar ile yarıştığı, çift 5x7 LED matris ekranlı elektronik zar sistemiyle desteklenen interaktif **"Sayı Avı"** konsolu.

---

<a id="icindekiler"></a>
## 📑 İçindekiler

1. [Proje Özeti ve Oyun Konsepti](#proje-ozeti)
2. [Donanım Mimarisi ve Devre Bağlantıları](#donanim-ve-baglantilar)
3. [I2C Adresleme ve Donanım Tablosu](#i2c-ve-donanimlar)
4. [Oyun Mekaniği ve Algoritma Mimarisi](#oyun-mekanigi)
5. [Elektronik Zar Modülü (zar.ino)](#elektronik-zar)
6. [Proje Dosya Yapısı](#dosya-yapisi)
7. [Kurulum ve Çalıştırma Rehberi](#kurulum-ve-calistirma)
8. [Geliştirici](#gelistirici)
9. [Lisans](#lisans)

---

<a id="proje-ozeti"></a>
## 🎮 1. Proje Özeti ve Oyun Konsepti

**Sayı Avı**, fiziksel kumanda ve ekran modülleriyle gerçek zamanlı çalışan iki oyunculu bir rekabet oyunudur:

- **İki Bağımsız Oyuncu İstasyonu:** Oyuncu 1 ve Oyuncu 2, kendilerine tahsis edilmiş **Deneyap OLED ekran** ve **Deneyap Kumanda Kolu (Joystick)** ile oyunu kontrol eder.
- **Sayı Matrisi:** Her oyuncunun OLED ekranında 1'den 12'ye kadar sayılar 3 satır x 4 sütun formatında listelenir.
- **Joystick İmleç Kontrolü:** Oyuncular analog joystick ile imleci sayılar üzerinde serbestçe gezdirir. Aktif seçili olan sayı ekranda yanıp sönerek (blink) görsel geri bildirim sağlar.
- **Hedef Avlama:** Joystick butonuna (`SW`) basıldığında ilgili sayı matristen elenir/silinir.
- **Merkezi Elektronik Zar:** Ortada yer alan iki adet **5x7 Kırmızı LED Matris**, fiziksel butona basıldığında `random(1, 7)` ile çift zar atar ve sayıları özel piksel desenleriyle görüntüler.
- **Kazanma ve Süre Hesabı:** 12 sayıyı da ilk tamamlayan oyuncunun bitiş süresi hesaplanarak kazanan ilan edilir.

---

<a id="donanim-ve-baglantilar"></a>
## 🔌 2. Donanım Mimarisi ve Devre Bağlantıları

Projede kullanılan tüm ekran ve kumanda bileşenleri **I2C haberleşme protokolü** üzerinden tek bir veri yoluna (SDA/SCL) zincirleme (daisy-chain) olarak bağlanmıştır.

| 📐 Deneyap I2C Bağlantı Şeması | 🎮 Çalışır Durumdaki Konsol Prototipi |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/6f3809e8-1680-4a99-ad7c-d482dcee3fce" alt="I2C Şeması" width="370" /> | <img src="https://github.com/user-attachments/assets/c6e831d4-c209-4440-8808-bfa96194d4a8" alt="Çalışan Prototip" width="370" /> |
| *Deneyap modülleri zincirleme bağlantı şeması* | *OLED, 5x7 LED Matris ve kumandalar aktif* |

---

<a id="i2c-ve-donanimlar"></a>
## 🗺️ 3. I2C Adresleme ve Donanım Tablosu

Aynı veri hattında birden fazla aynı tip modülün çakışmadan çalışabilmesi için her bir modülün I2C adresi donanımsal lehim köprüsü (jumper) ile farklılaştırılmıştır:

| Donanım Bileşeni | Adet | Görev | I2C Adresi / Pin |
| :--- | :---: | :--- | :--- |
| **Deneyap Kart** | 1 | Ana mikrodenetleyici & oyun döngüsü yöneticisi | ESP32 Çekirdek |
| **Deneyap OLED Ekran (1)** | 1 | 1. Oyuncu sayı matrisi & oyun durum ekranı | `0x7A` (I2C) |
| **Deneyap OLED Ekran (2)** | 1 | 2. Oyuncu sayı matrisi & oyun durum ekranı | `0x78` (I2C) |
| **Deneyap Kumanda Kolu (1)** | 1 | 1. Oyuncu analog yön & sayı seçme butonu | `0x1A` (I2C) |
| **Deneyap Kumanda Kolu (2)** | 1 | 2. Oyuncu analog yön & sayı seçme butonu | `0x1B` (I2C) |
| **5x7 Kırmızı LED Matris (1)** | 1 | 1. Zar rakam göstergesi | `0x0A` (I2C) |
| **5x7 Kırmızı LED Matris (2)** | 1 | 2. Zar rakam göstergesi | `0x4A` (I2C) |
| **Fiziksel Buton** | 1 | Elektronik zar atma tetikleyici | `D2` (INPUT_PULLUP) |

> 💡 **Mühendislik Notu:** I2C protokolü sayesinde yalnızca 2 adet sinyal pini (SDA & SCL) kullanılarak toplam **6 adet akıllı çevre birimi** (2 OLED + 2 Joystick + 2 Matris) minimum kablo karmaşası ile stabil biçimde çalıştırılmıştır.

---

<a id="oyun-mekanigi"></a>
## 🧠 4. Oyun Mekaniği ve Algoritma Mimarisi (`sayi_avi.ino`)

### İmleç Gezinme ve Debounce Kontrolü
Analog kumanda kollarının (`Joystick.xRead()`, `Joystick.yRead()`) hassasiyetini dengelemek ve istemsiz atlamaları önlemek için eşik değer filtresi ve zaman gecikmesi uygulanmıştır:

```cpp
// Hareket eşikleri ve gecikme filtresi (moveDelay = 200 ms)
if (xVal > 700) dx = 1;      // Sağa hareket
else if (xVal < 300) dx = -1;// Sola hareket
if (yVal > 700) dy = 1;      // Aşağı hareket
else if (yVal < 300) dy = -1;// Yukarı hareket
```

### Dinamik İmleç Yanıp Sönmesi (Blink Efekti)
Seçili hücrenin kullanıcı tarafından net şekilde anlaşılması için `millis()` tabanlı 300 milisaniyelik `blinkInterval` kullanılarak sayı düzenli periyotla görünür/gizlenir yapılır.

### Hedef Sayıyı Avlama
Oyuncu seçili sayı üzerindeyken kumanda kolunun merkez butonuna bastığında (`joystick.swRead()`):
1. Seçili sayının `visible[index]` durumu `false` yapılarak ekrandan silinir.
2. `silinenSayac` 1 artırılır.
3. İmleç bir sonraki görünür sayıya otomatik kaydırılır.
4. 12 sayının tamamı silindiğinde oyun sonlanır ve geçen toplam milisaniye/saniye ekrana yazdırılır.

---

<a id="elektronik-zar"></a>
## 🎲 5. Elektronik Zar Modülü (`zar.ino`)

Ortadaki iki adet 5x7 LED Matris ekran, bağımsız bir dijital çift zar modülü olarak çalışır:

- `D2` pinine bağlı butona basıldığında `random(1, 7)` fonksiyonu ile iki bağımsız rastgele sayı üretilir.
- 1'den 6'ya kadar olan rakamlar, 5x7 piksel matrisine uygun **özel ikili (binary) bit desenleri** ile oluşturulmuştur:

```cpp
static const byte digits[6][7] = {
  {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}, // 1
  {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}, // 2
  {0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110}, // 3
  {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}, // 4
  {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110}, // 5
  {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110}  // 6
};
```

---

<a id="dosya-yapisi"></a>
## 📁 6. Proje Dosya Yapısı

```text
Sayi_Avi/
├── sayi_avi/                # İki oyunculu konsol ana oyun yazılımı
│   └── sayi_avi.ino         # OLED ekranlar ve kumanda kollarını yöneten ana kod
├── zar/                     # Elektronik çift zar yazılımı
│   └── zar.ino              # 5x7 LED matrisler ve buton tetiklemeli zar kodu
├── assets/                  # Proje şema ve donanım prototip görselleri
└── README.md                # Proje teknik dokümantasyonu
```

---

<a id="kurulum-ve-calistirma"></a>
## 🚀 7. Kurulum ve Çalıştırma Rehberi

### 1. Gerekli Arduino Kütüphaneleri
Arduino IDE üzerinde **Araçlar ➔ Kütüphaneleri Yönet** menüsünden aşağıdaki resmi Deneyap kütüphanelerini yükleyin:
- `Deneyap_OLED`
- `Deneyap_KumandaKolu`
- `Deneyap_5x7LedEkran`

### 2. Donanım Bağlantısı
1. Tüm Deneyap modüllerini I2C ara kabloları ile zincirleme bağlayın.
2. 2. OLED ekranın arkasındaki adres köprüsünü değiştirerek `0x78` yapın (varsayılan: `0x7A`).
3. 2. Kumanda kolunun arkasındaki adres köprüsünü değiştirerek `0x1B` yapın (varsayılan: `0x1A`).
4. 2. LED Matrisin adres köprüsünü `0x4A` yapın (varsayılan: `0x0A`).

### 3. Kodu Yükleme
1. Arduino IDE'de **Kart:** `Deneyap Kart` (veya `Deneyap Kart 1A`) seçin.
2. `sayi_avi/sayi_avi.ino` dosyasını açıp karta yükleyin.
3. Ekranda 1'den 12'ye kadar sayılar belirecek ve iki oyunculu müsabaka başlayacaktır!

---

<a id="gelistirici"></a>
## 👩‍💻 8. Geliştirici

**Kevser ÜSTÜN**  
🎓 Giresun Üniversitesi — Bilgisayar Mühendisliği  
🔗 [GitHub Profilim](https://github.com/kvsrustun)

---

<a id="lisans"></a>
## 📄 9. Lisans

Bu proje [MIT Lisansı](LICENSE) kapsamında açık kaynak olarak sunulmuştur.
