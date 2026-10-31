// AtomS3R-Display-Probe.ino
//
// Purpose: identify whether an AtomS3R has a GC9107 or an ST7735 display.
// The display is on a write-only-ish 3-wire SPI bus (no MISO pin), so no
// host-side CLI (esptool, system_profiler, ...) can ask it directly.
// Code running ON the ESP32-S3 has to send the MIPI DCS RDDID command
// (0x04) over SPI and read the reply back on the shared MOSI/SDA line.
//
// This sketch does not do that read itself: M5GFX already does it during
// M5.begin(), and logs the result. Just upload, open the serial monitor
// at 115200, press reset, and look for one of:
//
//   [Autodetect] board_M5AtomS3R (ST7735)
//   [Autodetect] board_M5AtomS3R (GC9107)
//
// That line is the answer. (M5GFX source: src/M5GFX.cpp, AtomS3R case:
// ST7735 when the RDDID low 16 bits are 0x7683 or 0x897C -- the shifted
// form of the ST7735S 0x7C89F0 ID -- GC9107 when the low 24 bits are
// 0x079100. Some GC9107 batches only answer at a 100 kHz probe clock;
// M5GFX re-probes slowly before giving up.)
//
// IMPORTANT before flashing anything:
//   Back up the current flash first, e.g.:
//     python3 -m esptool --chip esp32s3 -p /dev/cu.usbmodemXXXX \
//       read-flash 0 0x800000 atoms3r-factory.bin
//   Flashing this sketch overwrites whatever firmware is on the device.
//
// Build (Arduino IDE, easiest for a one-off):
//   1. Boards Manager: install "esp32" by Espressif, select board M5AtomS3R.
//   2. Library Manager: install M5Unified (>= 0.2.17) and M5GFX (>= 0.2.22).
//   3. Tools > Core Debug Level > Info   (or higher -- without this the
//      [Autodetect] line, which is an ESP_LOGI message, will not print).
//   4. Upload, then Tools > Serial Monitor at 115200 baud, press reset.
//
// Build (arduino-cli, on an M4 Mac):
//   brew install arduino-cli
//   arduino-cli core install esp32:esp32
//   arduino-cli lib install M5Unified   # pulls in M5GFX
//   arduino-cli compile --fqbn esp32:esp32:M5AtomS3R --build-property \
//     build.extra_flags=-DCORE_DEBUG_LEVEL=3 AtomS3R-Display-Probe.ino
//   arduino-cli upload -p /dev/cu.usbmodemXXXX \
//     --fqbn esp32:esp32:M5AtomS3R AtomS3R-Display-Probe.ino
//   arduino-cli monitor -p /dev/cu.usbmodemXXXX -c baudrate=115200
//   (If the M5AtomS3R fqbn name differs in your installed core version,
//   use the Arduino IDE board selector instead of guessing the fqbn.)

#include <M5Unified.h>

void setup()
{
  Serial.begin(115200);
  delay(500);

  auto cfg = M5.config();
  M5.begin(cfg);

  // M5.begin() already ran M5GFX autodetection and logged the panel.
  // These lines are just a sanity check that the sketch is running and
  // that the display M5GFX picked can actually draw.
  Serial.printf("M5 board id: %d\n", (int)M5.getBoard());
  Serial.println("Look above for: [Autodetect] board_M5AtomS3R (ST7735|GC9107)");

  M5.Display.setTextSize(2);
  M5.Display.clear(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Probe running");
  M5.Display.println("Check serial");
  M5.Display.println("for panel ID");
}

void loop()
{
  M5.update();
  delay(100);
}
