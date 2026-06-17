# AGENTS.md

## Project Context

This is the clean GitHub-ready D320 repository.

Path:

`D:\Users\lyx\project\D320_0610\D320_github_clean`

Remote repository:

`https://github.com/lyxhahahalalala/D320.git`

This repository was created from the larger original checkout at:

`D:\Users\lyx\project\D320_0610`

The original checkout is about 23 GB and should not be pushed directly to GitHub.

## Git State

Initial local commit:

`23c7c2c Initial clean D320 project import`

The clean import intentionally excludes large binary assets, supplier installers, firmware history outputs, and generated build files.

## User Preference

The user will handle Git pull, push, authentication, and remote operations manually.

Do not run `git push`, `git pull`, or other network Git commands unless the user explicitly asks for it.

Local Git status checks, diffs, and commits are acceptable when requested.

## Ignore Policy

Do not commit:

- `*.psd`
- `*.zip`, `*.rar`, `*.7z`, `*.cab`, `*.msi`, `*.exe`, `*.dll`
- `*.hex`, `*.bin`, `*.mot`, `*.out`, `*.map`, `*.dep`, `*.dla`, `*.dnm`, `*.mem`, `*.rec`, `*.siz`, `*.lst`, `*.dbo`
- `*.log`, `*.tmp`, `*.bak`
- `*.tlog`, `*.recipe`, `*.lastbuildstate`, `BuildLog.htm`
- `output/`, `obj/`, `objects/`, `list/`, `History/`, `历史/`

If a future task needs one of these files, ask the user whether it should be kept outside GitHub, stored as a release asset, or handled through Git LFS.

## Workflow Notes

Use this directory for normal source edits.

Use the original checkout only as an archive/reference source.

Keep changes scoped. Avoid reintroducing the removed large folders from the original project.

## CAN Notes

Field testing on the current device showed both CAN channels communicating at 500K:

- CAN1 / BCAN: 500K
- CAN2 / PCAN: 500K

Although the code can select baud rates through `eol_icutype`, prefer this measured dual-500K setup when guiding bench tests and CAN message verification for the current hardware.

When adding a new PCAN receive ID in `02-Code\app\gfx_apps\simple_draw\src\can\rscan.c`, check both the receive rule table and the enabled rule count in `RSCAN0.GAFLCFG0.UINT32`. A new rule appended after the enabled CAN2 range can compile normally but still be blocked by hardware filtering.

This happened with new PCAN ID `0x04F02270`: the software table and signal logic were present, but `can_getPCanRxState(0x04F02270)` stayed at timeout state `2`. Replacing an earlier enabled PCAN rule proved the signal path worked. The real fix was to keep the new rule at the end and increase the CAN2 enabled receive rule count, for example from:

```c
RSCAN0.GAFLCFG0.UINT32 = 0x002D2D00;
```

to:

```c
RSCAN0.GAFLCFG0.UINT32 = 0x00302D00;
```

This enables enough CAN2 rules to cover the added `0x04F02270` filter entry.

## 0x04F02270 Current Status

The current `0x04F02270` bench-test setup is:

- Channel: CAN2 / PCAN
- Baud rate: 500K on the current hardware
- Frame type: extended CAN frame
- DLC: 8
- Recommended cycle: 100 ms

Known test payloads:

```text
00 00 00 00 00 00 00 00  clear all tested bits
01 00 00 00 00 00 00 00  VCUVehRdy / READY
10 00 00 00 00 00 00 00  Park switch Status
40 00 00 00 00 00 00 00  HVBattCutOff
80 00 00 00 00 00 00 00  EPS_Fault signal bit
00 01 00 00 00 00 00 00  VCUMotOverTemp
00 02 00 00 00 00 00 00  SOCLowWarning
```

Bench results so far:

- `Park switch Status` works after the PCAN hardware filter count fix.
- `HVBattCutOff` works.
- `VCUMotOverTemp` works.
- `SOCLowWarning` works.
- `READY` is not verified. The old READY message `0x18FFF531` also did not visibly trigger READY during this debug session, so READY is parked for now.
- `EPS_Fault` is received and can set `alarm_led[3].bit1` through `LED_ESC`, but this is the ESP/ESC lamp (`D334`), not a real EPS telltale. The project currently does not have the expected EPS icon/display resource wired for this signal, so EPS display is parked for now.
- `EPFaultLevelWarning` is intentionally not wired to `LED_SysFault` yet.
- `LowBatteryWarning` is defined in the new `VCU_04F02270_t` structure but is not wired to a display path yet. The old low-voltage warning is not CAN-driven; it comes from local `BAT_V` sampling via `get_aux_bat_volt() <= 210` for at least 3 seconds.

Useful debug checks from MULTI:

```text
print can_getPCanRxState(0x04F02270)
print can_getPCanRxState(0x18FEF131)
print alarm_led[5].byte
print alarm_led[3].byte
```

During debugging, `can_getPCanRxState(0x04F02270) == 2` meant the new message was still timing out. `can_getPCanRxState(0x18FEF131) == 1` at the same time proved PCAN itself was working and narrowed the issue to the new hardware receive filter range.

If a new CAN ID does not enter `Process_Can2()`, test with an old working PCAN frame such as `0x18FEF131` at the same breakpoint. If the old frame enters immediately and the new frame does not, focus on `rscan.c` hardware filtering before changing display logic.

## File Modification Rule

Do not modify, create, delete, format, or overwrite any project file unless the user has explicitly approved that specific file change first.

Reading files, searching code, checking local Git status, and explaining findings are allowed. Before any edit, state the intended file path and change, then wait for the user's approval.

## Coding Style Preference

When giving code suggestions or implementing approved functional changes, follow the original project's existing style as much as possible. Prefer the current naming, file organization, global flag patterns, periodic task style, CAN table style, and direct C implementation approach already used in the project.

Avoid introducing new abstractions, formatting styles, helper layers, or architectural changes unless the user explicitly asks for them or they are necessary for the requested change.

## PDF Reading

When a PDF needs to be inspected, use the `pdftotext` tool from:

`C:\Users\Jasmine\Desktop\Beyond_Compare_4.4.4.27058_64bit_Green`

Prefer converting the relevant PDF pages/text locally with that tool instead of guessing from the filename.
