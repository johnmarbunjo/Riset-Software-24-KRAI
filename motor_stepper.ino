const int enPin   = 22;  // Pin Enable
const int dirPin  = 23;  // Pin Direction (Arah)
const int stepPin = 24; // Pin Step (Pulsa)

// Setingan jumlah langkah per putaran motor
// Tergantung pada sakelar DIP/microstepping di driver TB6560
// NEMA standar biasanya 200 langkah per revolusi (1.8 derajat per step)
const int stepsPerRev = 200; 

void setup() {
  // Atur semua pin kontrol sebagai OUTPUT
  pinMode(enPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  // Pada TB6560, memberikan sinyal LOW pada pin Enable 
  // akan MENGAKTIFKAN motor (motor akan terkunci menahan posisi).
  // Sinyal HIGH akan membuat motor bebas/lepas (freewheel).
  digitalWrite(enPin, LOW); 
  
  Serial.begin(9600);
  Serial.println("Driver TB6560 Siap!");
}

void loop() {
  // --- 1. Putar Searah Jarum Jam (Clockwise) ---
  Serial.println("Putar Kanan...");
  digitalWrite(dirPin, HIGH); // Set arah putaran
  
  for(int i = 0; i < stepsPerRev; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);    // Waktu nyala pulsa (atur kecepatan di sini)
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);    // Waktu mati pulsa
    // Semakin KECIL angka delayMicroseconds, putaran motor semakin CEPAT
  }
  
  delay(1000); // Jeda 1 detik setelah selesai 1 putaran


  // --- 2. Putar Berlawanan Arah Jarum Jam (Counter-Clockwise) ---
  Serial.println("Putar Kiri...");
  digitalWrite(dirPin, LOW);  // Ubah arah putaran
  
  for(int i = 0; i < stepsPerRev; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  
  delay(1000); // Jeda 1 detik
}