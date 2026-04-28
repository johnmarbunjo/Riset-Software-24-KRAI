#include <Servo.h>

Servo myServo;

// === KONFIGURASI ===
const int SERVO_PIN   = 9;
const int MIN_PULSE   = 500;   // μs → 0°
const int MAX_PULSE   = 2500;  // μs → 270°
const int MAX_ANGLE   = 270;

// Posisi sudut untuk setiap tombol
// Kamu bisa ubah nilai-nilai ini sesuai kebutuhan
struct KeyAngle {
  char    key;
  int     angle;
  String  label;
};

KeyAngle mapping[] = {
  { 'a',   0,   "Posisi 0°   (Min)"     },
  { 's',  45,   "Posisi 45°"            },
  { 'd',  90,   "Posisi 90°"            },
  { 'f', 135,   "Posisi 135° (Tengah)"  },
  { 'g', 180,   "Posisi 180°"           },
  { 'h', 225,   "Posisi 225°"           },
  { 'j', 270,   "Posisi 270° (Max)"     },
};

const int TOTAL_KEYS = sizeof(mapping) / sizeof(mapping[0]);
int currentAngle = 0;

// =============================================
void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN, MIN_PULSE, MAX_PULSE);

  // Posisi awal
  myServo.write(0);
  currentAngle = 0;
  delay(500);

  printMenu();
}

// =============================================
void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();

    // Abaikan karakter newline/carriage return
    if (input == '\n' || input == '\r') return;

    bool found = false;

    // Cari di tabel mapping
    for (int i = 0; i < TOTAL_KEYS; i++) {
      if (input == mapping[i].key) {
        moveServo(mapping[i].angle, mapping[i].label);
        found = true;
        break;
      }
    }

    // Tombol spesial
    if (!found) {
      if (input == '+') {
        // Naik 10° dari posisi sekarang
        int newAngle = constrain(currentAngle + 10, 0, MAX_ANGLE);
        moveServo(newAngle, "Naik 10°");
      }
      else if (input == '-') {
        // Turun 10° dari posisi sekarang
        int newAngle = constrain(currentAngle - 10, 0, MAX_ANGLE);
        moveServo(newAngle, "Turun 10°");
      }
      else if (input == 'm') {
        printMenu();
      }
      else {
        Serial.print("Tombol '");
        Serial.print(input);
        Serial.println("' tidak terdaftar. Tekan 'm' untuk menu.");
      }
    }
  }
}

// =============================================
void moveServo(int targetAngle, String label) {
  targetAngle = constrain(targetAngle, 0, MAX_ANGLE);

  Serial.print(">> ");
  Serial.print(label);
  Serial.print(" → Bergerak ke ");
  Serial.print(targetAngle);
  Serial.println("°");

  // Gerak perlahan (smooth) dari posisi sekarang ke target
  if (targetAngle > currentAngle) {
    for (int pos = currentAngle; pos <= targetAngle; pos++) {
      myServo.write(pos);
      delay(8); // Kecepatan gerak: perbesar = makin lambat
    }
  } else {
    for (int pos = currentAngle; pos >= targetAngle; pos--) {
      myServo.write(pos);
      delay(8);
    }
  }

  currentAngle = targetAngle;
  Serial.print("   Posisi sekarang: ");
  Serial.print(currentAngle);
  Serial.println("°\n");
}

// =============================================
void printMenu() {
  Serial.println("\n=============================");
  Serial.println("  KONTROL SERVO DS3235 SG");
  Serial.println("=============================");
  for (int i = 0; i < TOTAL_KEYS; i++) {
    Serial.print(" [");
    Serial.print(mapping[i].key);
    Serial.print("] → ");
    Serial.println(mapping[i].label);
  }
  Serial.println(" [+] → Naik 10°");
  Serial.println(" [-] → Turun 10°");
  Serial.println(" [m] → Tampilkan menu");
  Serial.println("=============================\n");
}