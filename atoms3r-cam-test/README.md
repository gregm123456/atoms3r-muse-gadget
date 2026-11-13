# AtomS3R-CAM Grove button test

This standalone Arduino sketch checks a normally open, active-low button on
the Grove port. It does not initialize the camera or capture images.

## Wiring and operation

1. Connect the Grove yellow wire (G2) briefly to the black wire (GND).
   Disconnect them to release. Use insulated leads and avoid accidental
   contact with any other pin. Leave red (5 V) and white (G1) disconnected;
   never connect 5 V to a GPIO.
2. The built-in camera power indicator lights while G2 is connected to GND
   and goes out on release. No wiring to GPIO18 is needed. The camera is
   powered while the light is on.
3. Optional: open the USB serial monitor at 115200 baud for button state
   changes and a status message every five seconds.

## Result on one unit

On November 7, 2026, connecting and disconnecting G2 and GND controlled the
indicator. After an initially observed multi-second delay of unknown cause,
the indicator responded apparently immediately; USB logs also captured
multiple press/release cycles within one second. Exact latency was not
measured. After a later power cycle, repeated tests again showed multi-second
delays. The sketch aims to sample every 5 ms and requires 30 ms of stable
input, but its synchronous USB serial writes can block the same loop when the
host is not reading. This is a plausible cause, not yet confirmed; do not
treat the test as a latency measurement. The test checks the electrical
connection and indicator, not Muse pairing, reset, or the SDK's button timing.

## Build and upload

For an already installed Arduino ESP32 core, build and upload with:

```sh
arduino-cli compile --fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc,FlashSize=8M,PartitionScheme=default_8MB atoms3r-cam-test
arduino-cli upload --port /dev/cu.usbmodemXXXX --fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc,FlashSize=8M,PartitionScheme=default_8MB atoms3r-cam-test
```

Uploading replaces the existing firmware. Back up the device first if its
contents matter; a backup may contain credentials and must remain private.

## Restore the previous firmware

For the tested unit, the complete 8 MB flash was backed up before this test
to the ignored `backups/atoms3r-cam-before-grove-test.bin`. With the same
device connected and the repository's local esptool installed, restore it
from the repository root:

```sh
venv/bin/python -m esptool --port /dev/cu.usbmodemXXXX write-flash 0x0 backups/atoms3r-cam-before-grove-test.bin
```

Replace the port placeholder with the device's current port. This overwrites
the entire flash, including settings and any changes made since the backup;
use only the backup belonging to that device. The tested device had flash
encryption and secure boot disabled. Do not assume this restore procedure
works for an encrypted or secure-boot-enabled device.
