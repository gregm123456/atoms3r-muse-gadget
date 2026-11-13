// Grove G2-to-GND button test for the AtomS3R-CAM.
// The camera power indicator lights while the button is pressed.

constexpr int BUTTON_PIN = 2;
constexpr int CAMERA_POWER_PIN = 18;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long STATUS_MS = 5000;

bool rawPressed;
bool pressed;
unsigned long lastChange;
unsigned long lastStatus;

void setup()
{
  // GPIO18 is active-low; set the output latch before enabling output.
  digitalWrite(CAMERA_POWER_PIN, HIGH);
  pinMode(CAMERA_POWER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  rawPressed = digitalRead(BUTTON_PIN) == LOW;
  pressed = false;
  lastChange = millis();
  lastStatus = millis();
  Serial.println("Grove button test: connect G2 to GND to turn on camera LED.");
}

void loop()
{
  const unsigned long now = millis();
  const bool reading = digitalRead(BUTTON_PIN) == LOW;
  if (reading != rawPressed) {
    rawPressed = reading;
    lastChange = now;
  }
  if (rawPressed != pressed && now - lastChange >= DEBOUNCE_MS) {
    pressed = rawPressed;
    digitalWrite(CAMERA_POWER_PIN, pressed ? LOW : HIGH);
    Serial.println(pressed ? "PRESSED: camera LED on" : "RELEASED: camera LED off");
  }
  if (now - lastStatus >= STATUS_MS) {
    lastStatus = now;
    Serial.println(pressed ? "Status: pressed" : "Status: released");
  }
  delay(5);
}
