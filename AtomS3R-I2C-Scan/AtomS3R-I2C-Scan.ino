// AtomS3R-I2C-Scan.ino
// Phase 0 evidence: scan both I2C buses on AtomS3R + Voice Base and read ID
// registers. Read-only; writes nothing to any peripheral.
//   internal bus: SDA G45, SCL G0  (BMI270, LP5562, ...)
//   base bus:     SDA G38, SCL G39 (ES8311, PI4IOE5V6408, ...)
// Output is repeated every 5 s on USB CDC at 115200.
#include <Wire.h>

static int readReg(TwoWire &w, uint8_t addr, uint8_t reg)
{
  w.beginTransmission(addr);
  w.write(reg);
  if (w.endTransmission(false) != 0) return -1;
  if (w.requestFrom((int)addr, 1) != 1) return -1;
  return w.read();
}

static void scan(const char *name, TwoWire &w, int sda, int scl)
{
  w.begin(sda, scl, 100000);
  Serial.printf("== %s (SDA G%d, SCL G%d)\n", name, sda, scl);
  int n = 0;
  for (uint8_t a = 1; a < 127; a++) {
    w.beginTransmission(a);
    if (w.endTransmission() == 0) {
      Serial.printf("  found 0x%02X\n", a);
      n++;
    }
  }
  if (!n) Serial.println("  (nothing)");
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
}

void loop()
{
  Serial.println("---- I2C scan ----");
  scan("internal", Wire, 45, 0);
  Serial.printf("  BMI270 0x68 CHIP_ID(0x00)=0x%02X (expect 0x24)\n", readReg(Wire, 0x68, 0x00));
  Serial.printf("  LP5562 0x30 reg 0x00=0x%02X\n", readReg(Wire, 0x30, 0x00));
  Wire.end();
  scan("base", Wire1, 38, 39);
  Serial.printf("  ES8311 0x18 CHIP_ID1(0xFD)=0x%02X CHIP_ID2(0xFE)=0x%02X (expect 0x83 0x11)\n",
                readReg(Wire1, 0x18, 0xFD), readReg(Wire1, 0x18, 0xFE));
  Wire1.end();
  Serial.println("---- end ----");
  delay(5000);
}
