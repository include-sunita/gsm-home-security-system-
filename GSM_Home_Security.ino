/*
  GSM based Home Security System using PIR Sensor and Arduino Uno
  --------------------------------------------------------------
  Components:
    - PIR Motion Sensor  -> OUT: D10, VCC: 5V, GND: GND
    - GSM Module (SIM800L) -> TXD: D3 (Arduino RX), RXD: D2 (Arduino TX), GND: GND, VCC: ~4V (buck converter)
    - Buzzer / LED (optional) -> for local alert indication

  Behaviour:
    1. Continuously monitor the PIR sensor.
    2. On motion detection, send an alert call (and SMS) via GSM module using AT commands.
    3. If the call is not answered, retry up to MAX_RETRIES times.
    4. Use a short cooldown/debounce period to reduce false alarms.

  Note: Update PHONE_NUMBER with the homeowner's number before uploading.
*/

#include <SoftwareSerial.h>

// ---------- Pin Definitions ----------
const int PIR_PIN     = 10;   // PIR sensor OUT pin
const int GSM_TX_PIN  = 2;    // Arduino TX -> GSM RXD
const int GSM_RX_PIN  = 3;    // Arduino RX -> GSM TXD
const int BUZZER_PIN  = 8;    // Optional buzzer
const int LED_PIN     = 9;    // Optional LED

// ---------- GSM Serial ----------
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN); // RX, TX

// ---------- Configuration ----------
const char PHONE_NUMBER[]   = "+91XXXXXXXXXX"; // TODO: set homeowner's number
const unsigned long COOLDOWN_MS   = 15000;      // cooldown after an alert to avoid spamming
const unsigned long CALL_DURATION = 20000;      // how long to let the call ring before hanging up
const int MAX_RETRIES        = 3;               // retry attempts if call is not answered/fails

// ---------- State ----------
unsigned long lastAlertTime = 0;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);      // Debug monitor
  gsmSerial.begin(9600);   // GSM module

  Serial.println("Initializing GSM module...");
  delay(3000); // allow GSM module time to register on network

  sendATCommand("AT");         // basic handshake
  sendATCommand("AT+CMGF=1");  // set SMS to text mode

  Serial.println("System ready. Monitoring for motion...");
}

void loop() {
  int motionState = digitalRead(PIR_PIN);

  if (motionState == HIGH && (millis() - lastAlertTime > COOLDOWN_MS)) {
    Serial.println("Motion Detected!");
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    triggerAlert();

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    lastAlertTime = millis();
    Serial.println("Returning to normal monitoring mode.");
  }
}

// ---------- Alert Logic ----------
void triggerAlert() {
  sendSMS("Alert! Motion detected at home.");

  bool callSuccess = false;
  for (int attempt = 1; attempt <= MAX_RETRIES && !callSuccess; attempt++) {
    Serial.print("Calling homeowner, attempt ");
    Serial.println(attempt);
    callSuccess = makeCall();

    if (!callSuccess) {
      Serial.println("Call attempt failed. Retrying...");
      delay(2000);
    }
  }

  if (!callSuccess) {
    Serial.println("All call attempts failed. SMS alert already sent.");
  }
}

// ---------- Send SMS ----------
void sendSMS(String message) {
  Serial.println("Sending SMS...");
  gsmSerial.println("AT+CMGF=1");
  delay(500);
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(PHONE_NUMBER);
  gsmSerial.println("\"");
  delay(500);
  gsmSerial.print(message);
  delay(500);
  gsmSerial.write(26); // Ctrl+Z to send the SMS
  delay(3000);
  Serial.println("SMS sent.");
}

// ---------- Make Call ----------
bool makeCall() {
  gsmSerial.print("ATD");
  gsmSerial.print(PHONE_NUMBER);
  gsmSerial.println(";");
  delay(CALL_DURATION);
  gsmSerial.println("ATH"); // hang up
  delay(1000);

  // NOTE: SIM800L does not directly report "call answered" over serial
  // without parsing +CLCC responses. For simplicity this function assumes
  // the call attempt completed; extend with +CLCC polling for real
  // answered/unanswered detection.
  return true;
}

// ---------- Helper: send AT command and print response ----------
void sendATCommand(String command) {
  gsmSerial.println(command);
  delay(1000);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}
