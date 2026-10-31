# AtomS3R Muse Gadget

Custom firmware and hardware notes for using the M5Stack AtomS3R with its
Atomic Voice Base as a Muse gadget.

## Hardware

- M5Stack AtomS3R with 8 MB flash and 8 MB PSRAM
- Atomic Voice Base with microphone, speaker, and ES8311 audio codec
- 128 × 128 display; the tested unit uses an ST7735 panel

The sketches in this repository document the display probe and I²C scan used
to identify and verify the hardware. Board revisions may differ; check your
own hardware before relying on the tested panel or peripheral details.

## SDK and firmware status

The firmware is based on Meta's
[Muse Gadget SDK](https://github.com/facebookincubator/muse-gadget-sdk).
AtomS3R board support lives in a
[personal fork](https://github.com/gregm123456/muse-gadget-sdk) of the SDK
(branch `atoms3r-port`), included here as a Git submodule at `muse-gadget-sdk/`.
The submodule pins the exact SDK revision used to build the firmware.

Clone with submodules:

```sh
git clone --recurse-submodules https://github.com/gregm123456/atoms3r-muse-gadget.git
```

Then follow the SDK's ESP32 setup instructions and build the AtomS3R target
from `muse-gadget-sdk/esp32/`:

```sh
tools/muse/board.sh build atoms3r
```

The build requires an SDK token. Enter it only in the local, ignored ESP-IDF
`sdkconfig` (as `CONFIG_GADGET_SDK_TOKEN`); never commit that file, paste the
token into source, or publish a firmware build containing a personal token.
Review the [Muse Gadget SDK Terms](https://gadgets.muse.ai/sdk-terms) before
pairing a device.

## Local files and secrets

The root `.gitignore` excludes local environments, build output, the factory
flash backup, personal notes, and credentials and key files. Review
the staged file list before committing, and do not force-add ignored local
configuration or backups.

## License

Original project files in this repository (the README and the sketches) are
offered under the MIT License; see [LICENSE](LICENSE). The Muse Gadget SDK and
other third-party material are not relicensed by this project: retain their
original licenses, copyright notices, and attribution requirements when
including or distributing them.

## Credits and third-party licenses

- **[Muse Gadget SDK](https://github.com/facebookincubator/muse-gadget-sdk)**,
  Copyright (c) Meta Platforms, Inc. and affiliates, Apache License 2.0. The
  `muse-gadget-sdk/` submodule is a fork of it; its `LICENSE`, file headers and
  third-party license table (in its README) apply to everything in it, including
  my AtomS3R board port, which keeps the Apache-2.0 header so it fits the SDK.
- **[M5GFX](https://github.com/m5stack/M5GFX) and
  [M5Unified](https://github.com/m5stack/M5Unified)**, MIT License,
  Copyright (c) 2021 M5Stack. The AtomS3R board port's ST7735S init sequence and
  panel, backlight and audio-codec values are adapted from them, and the probe
  sketch uses them. The MIT notice is kept in the board file's header.
- **[Bosch Sensortec BMI270_SensorAPI](https://github.com/boschsensortec/BMI270_SensorAPI)**,
  BSD-3-Clause, Copyright (c) 2023 Bosch Sensortec GmbH. The BMI270 motion
  sensor's configuration blob (`atoms3r_bmi270_config.h` in the SDK fork) comes
  from it, with its notice in the file.
- This is an independent project, not affiliated with or endorsed by Meta or
  M5Stack. Product and company names belong to their owners.
