# Instructions for the project coding agent: repo layout changed

This file is local-only: normally keep it in the git-ignored `notes/` folder,
even if temporarily placed at the repo root. Do not commit it. It supersedes
any older assumption that `muse-gadget-sdk/` is a throwaway clone of upstream.

## What changed

- The project is a **public Git repo**: `gregm123456/atoms3r-muse-gadget`
  (this folder's root; default branch `main`).
- `muse-gadget-sdk/` is now a **Git submodule** pointing at Greg's **fork**
  `gregm123456/muse-gadget-sdk`; `.gitmodules` names `atoms3r-port`, but the
  outer commit pins an exact SDK commit, not a branch tip.
- All AtomS3R firmware work (board file, sdkconfig overlay, features, fixes)
  lives in the fork. The outer repo only records *which fork commit* it uses.
- **Never open PRs or push to upstream** (`facebookincubator/*`). Upstream is
  read-only. Greg does not want to contribute upstream.

The tested November 2026 upstream integration was pushed to SDK branch
`merge-upstream-2026-11-06` and outer branch `merge-upstream-muse-sdk`. At that
time, `atoms3r-port` and outer `main` were not updated. Check the current
branches and commit ancestry before starting work; do not blindly switch to
either older branch and lose sight of the integrated work.

## Remotes inside `muse-gadget-sdk/`

In this local clone:

- `origin` = upstream `facebookincubator/muse-gadget-sdk` (fetch only; never push)
- `personal` = Greg's fork (push here)

The recorded `.gitmodules` URL is the fork; a fresh submodule clone may instead
name the fork `origin` and have no upstream remote. Inspect `git remote -v`
and verify URLs before fetching or pushing. Add a clearly named upstream
remote if needed. Push explicitly to the verified fork remote and intended
branch, never by assuming a remote name or using a bare `git push`.

## Workflow for any firmware change (two commits, in this order)

1. In `muse-gadget-sdk/`: check the outer repo's pinned commit and choose the
   intended development branch containing it. A submodule may be on a detached
   HEAD; do not assume `atoms3r-port` contains the newest work. Edit, build,
   test, commit, then push the branch to the verified fork remote.
2. In the outer repo: `git add muse-gadget-sdk` (this records the new submodule
   commit), commit with a message naming the SDK change, then push the intended
   outer branch to its verified remote.

**Push the fork before pushing the outer repo.** Otherwise the outer repo points
at a commit that doesn't exist on GitHub and fresh clones break.

Check before finishing a task: `git -C muse-gadget-sdk status` is clean and
`git status` in the outer repo shows no modified `muse-gadget-sdk` pointer.

## Pulling in upstream updates (only when Greg asks)

1. Inspect both worktrees, branch tips, submodule pin and remotes. Fetch from
   the verified upstream remote and compare the fork with upstream `main`,
   including their merge base. A shallow clone may need additional history.
2. Create isolated SDK and outer branches from the intended development tips.
   Merge upstream into the SDK branch; do not rebase or rewrite published
   history without explicit approval. Resolve conflicts while retaining both
   upstream board additions and AtomS3R support.
3. Check upstream's current ESP-IDF prerequisite and configuration changes
   before building. Generated `sdkconfig` files can retain stale settings and
   secrets: preserve the old file, regenerate defaults in a separate build
   directory, and transfer only needed local settings privately. Never print
   or commit tokens. Verify critical settings (including TLS CA selection)
   and image size. `tools/muse/board.sh` uses a fixed per-board build directory;
   use its `IDF_EXPORT` override for a nonstandard IDF path, and use a separate
   `idf.py -B` build/config when isolation is needed.
4. Run relevant upstream host tests, including board isolation, diagnostic
   log tags, TLS and AtomS3R IMU/gestures. Build the actual AtomS3R firmware;
   report failures and distinguish pre-existing failures from merge regressions.
   With permission, flash the intended device without a full erase, verify boot,
   network, sensor and UI, and ask Greg to check physical interactions. Report
   what was not tested; a host test or boot log is not a physical gesture test.
5. Commit and push the SDK branch to the fork before committing and pushing
   the outer branch's submodule pointer. Keep changes on those branches unless
   Greg explicitly chooses how to integrate them into the usual branches.

Check disk space before installing toolchains or building. Prefer a separate
ESP-IDF checkout; changing a shared checkout can affect other projects and
requires Greg's approval. Verify the activated version rather than trusting its
directory name. The November integration required v6.0.3 and a fresh config
to pick up the common CA bundle; check requirements again on future updates.

Keep our edits to upstream files small and localized (board registration lines
in `Kconfig`, `CMakeLists.txt`, `idf_component.yml`, `board.sh`, `ports.py`,
`avatar.py`, docs) to keep merges easy. Put new logic in new files where possible.

## Licensing rules (public repo, keep legal)

- Outer repo original files: **MIT**, `LICENSE`, (c) Greg Merritt.
- Everything under `muse-gadget-sdk/`: **Apache-2.0**, (c) Meta. New SDK files
  (e.g. `board_*.c`) keep the Apache header already used in the SDK.
- `board_m5stack_atoms3r.c` contains values adapted from **M5GFX / M5Unified
  (MIT, (c) 2021 M5Stack)**. Keep that notice in the file header. If you port
  more code from M5GFX/M5Unified or any other project, add its notice to the file
  and a row to the SDK README's third-party license table, and mention it in the
  outer README "Credits" section.
- Don't copy code from sources with incompatible or unknown licenses.
- Preserve existing copyright headers and the upstream LICENSE.

## Secrets and privacy: hard rules (the repo is PUBLIC)

- **Never commit the SDK token** (`mgst_...`), Wi-Fi credentials, API keys or
  device tokens. The token goes only in the ignored `build-*/sdkconfig`
  (`CONFIG_GADGET_SDK_TOKEN`). Don't put it in overlays, `sdkconfig.defaults`,
  scripts, docs, commit messages, or logs you paste anywhere. Boot logs print
  the first 12 characters of it; don't commit logs.
- Don't commit: `build-*/`, `sdkconfig`, `*.pem` (we added `*.pem` to the outer
  `.gitignore`; the upstream-tracked `esp32/dev_signing_key.pem` is a shared dev
  key that already exists upstream, don't replace or add other keys), `venv/`,
  `atoms3r-factory.bin` (factory flash backup), serial logs.
- Don't write personal or household details into any tracked file or commit
  message: the owner's name beyond the LICENSE/commit author, home devices,
  rooms, SSIDs, serial port paths such as `/dev/cu.usbmodem...`, BLE device IDs
  like `MuseGadget-XXXXXX`. Use placeholders (`/dev/cu.usbmodemXXXX`).
- The plan and build log are kept **outside** the repo (or in `notes/`); never
  move them into tracked paths. `notes/` is git-ignored: do not remove that rule.
- Before any commit or push: review `git diff --cached`, and scan for
  `mgst_`, `PRIVATE KEY`, passwords, SSIDs, `usbmodem`. Don't use `git add -A`
  or `git add .` in the outer repo without reviewing `git status`; never
  `git add -f` an ignored file.
- If a secret is ever committed, stop, tell Greg, and don't just delete it in a
  later commit: history needs rewriting and the secret must be revoked.

## Where things live

| Thing | Location |
|---|---|
| Board port | `muse-gadget-sdk/esp32/components/muse/boards/board_m5stack_atoms3r.c` |
| Config overlay | `muse-gadget-sdk/esp32/devices/sdkconfig.muse-m5stack-atoms3r` |
| Build / flash | `muse-gadget-sdk/esp32/tools/muse/board.sh build\|flash atoms3r` |
| Probe and scan sketches | outer repo root |
| Public docs | outer `README.md` (keep it accurate and free of personal details) |
| Private plan and log | local only, outside the repo |

Fresh clone: `git clone --recurse-submodules https://github.com/gregm123456/atoms3r-muse-gadget.git`
(or `git submodule update --init` in an existing clone).
