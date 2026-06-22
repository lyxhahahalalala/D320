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

## 0x04F02370 Current Status

The `0x04F02370` requirement is implemented and bench-tested successfully.

Current setup:

- Channel: CAN2 / PCAN
- Baud rate: 500K on the current hardware
- Frame type: extended CAN frame
- DLC: 8
- Recommended cycle: 100 ms
- Direction: instrument receives this frame

Signals handled from this frame:

- `batt_soc`: byte0, battery SOC, resolution 0.4%, displayed as percent.
- `chg_line_sts`: byte1 bit0-bit1, charge line connection state.
- `charging_indication`: byte1 bit2-bit3, charging status.
- `pack_current`: byte2-byte3, little-endian, resolution 0.1 A, offset -1000 A.
- `pack_voltage`: byte4-byte5, little-endian, resolution 0.1 V.
- `abs_vehicle_speed`: byte6-byte7, little-endian, resolution 0.1 km/h.

Bench results:

- SOC update works.
- Charge line connection state works.
- Charging status works.
- Battery pack current works.
- Battery pack voltage works.
- Vehicle speed works.

Known working test frames:

```text
ID:   0x04F02370
Data: C8 05 8B 27 00 15 63 01
Expected: SOC 80%, A gun connected, Charging, current 12.3 A, voltage 537.6 V, speed 35.5 km/h

ID:   0x04F02370
Data: 00 00 10 27 00 00 00 00
Expected: SOC 0%, charge line not connected, NotCharged, current 0 A, voltage 0 V, speed 0 km/h

ID:   0x04F02370
Data: FA 0B 10 27 88 13 00 00
Expected: SOC 100%, dual gun connected, Charge Finished, current 0 A, voltage 500.0 V, speed 0 km/h

ID:   0x04F02370
Data: 80 0D 10 27 88 13 00 00
Expected: SOC about 51%, A gun connected, Charge error, current 0 A, voltage 500.0 V, speed 0 km/h
```

Useful MULTI debug checks:

```text
print can_getPCanRxState(0x04F02370)
print CAN_CHARGE_LINE
print ((VCU_04F02370_t*)can_getPCanBuffer(0x04F02370))->chg_line_sts
```

Expected charge line values:

```text
CAN_CHARGE_LINE:
0 = not connected
1 = connected

chg_line_sts:
0 = not connected
1 = A gun connected
2 = B gun connected
3 = dual gun connected
```

When adding both `0x04F02270` and `0x04F02370`, make sure the PCAN receive rule table entries in `rscan.c` use unique receive buffer indexes. Do not duplicate the third field. The known-good pattern is:

```c
{0x0CF19682UL, 0x1FFFFFFFUL, 0x00005A00UL, 0x00000004UL}, /*47*/
{0x04F02270UL, 0x1FFFFFFFUL, 0x00005B00UL, 0x00000004UL}, /*48*/
{0x04F02370UL, 0x1FFFFFFFUL, 0x00005C00UL, 0x00000004UL}, /*49*/
```

For this rule count, the hardware filter count should cover the added PCAN entries. The tested value was:

```c
RSCAN0.GAFLCFG0.UINT32 = 0x00312D00;
```

If `can_getPCanRxState(0x04F02370)` stays at `2`, first check the hardware filter table and `GAFLCFG0` count before changing the signal parsing or display logic.

## Vehicle Information Menu Status

The Vehicle Information menu in `02-Code\app\gfx_apps\simple_draw\src\main\DISPLAY.c` now includes the new `ControllerFault` page, displayed as:

```text
查看控制器故障
Controller faults
```

Normal vehicle layout:

- 11 menu items.
- Two columns.
- Left column has 6 items and right column has 5 items.
- Row spacing is 45 pixels.
- Valid `ichoose` range is `0` through `10`.
- `ichoose == 10` enters `ControllerFault`.

When `eol_fadongjiorkaji == 0x2d`:

- 13 menu items.
- Two columns with 45-pixel row spacing.
- Valid `ichoose` range is `0` through `12`.
- `ichoose == 12` enters `ControllerFault`.

When adding another menu item, update all related parts together:

- Menu text array.
- Draw loop item count.
- Forward and backward cursor boundary.
- Cursor coordinate calculation.
- `key_set` page mapping.
- `frame_ID` enumeration and `LCD_Exec()` switch branch.

Do not update only the displayed text. An item can be visible but impossible to select if the cursor range and `key_set` mapping are not updated.

## 0x04F02470 Controller Fault Status

The instrument receives controller fault values through:

- ID: `0x04F02470`
- Channel: CAN2 / PCAN
- Baud rate: 500K on the current hardware
- Frame type: extended CAN frame
- DLC: 8
- Recommended cycle: 100 ms
- Software receive timeout: 3000 ms

Current byte layout:

```text
byte0-byte1  EPS_Sys_Fault_Code
             steering DCAC system fault code, uint16 little-endian

byte2-byte3  ACM_Sys_Fault_Code
             braking DCAC system fault code, uint16 little-endian

byte4        DriveMCU_Sys_Fault_Code
byte5        StirMCU_Sys_Fault_Code
byte6        SuctionHeadMCU_Sys_Fault_Code
byte7        FrontConveyorbeltMCU_Sys_Fault_Code
```

The receive structure is `VCU_04F02470_t` in `app_can.h`. The ID is present in `lct_PcanRxFrame[]` in `app_can.c`.

The PCAN hardware filter entries currently end with:

```c
{0x04F02270UL, 0x1FFFFFFFUL, 0x00005B00UL, 0x00000004UL}, /*48*/
{0x04F02370UL, 0x1FFFFFFFUL, 0x00005C00UL, 0x00000004UL}, /*49*/
{0x04F02470UL, 0x1FFFFFFFUL, 0x00005D00UL, 0x00000004UL}, /*50*/
```

The matching CAN2 rule count is:

```c
RSCAN0.GAFLCFG0.UINT32 = 0x00322D00;
```

If another PCAN ID is appended, use the next unique rule index and increase the enabled CAN2 rule count again. Do not reuse `0x00005D00`.

## Controller Fault Page Behavior

The controller fault page is implemented through:

```c
ControllerFaultFrame()
ControllerFaultFrameUpdate()
```

The page follows these rules:

- A fault code equal to `0` is treated as no fault and its description is not displayed.
- A nonzero fault code displays its description and numeric value in red.
- Valid faults are packed from the first row downward with no blank rows between them.
- If `0x04F02470` times out, no fault item is displayed.
- Timeout is not displayed as `--` on this page.
- The displayed fault total is the number of nonzero fault codes currently received.
- The fault total is positioned below the lower horizontal line at approximately `Y = 438`.

Known test frame:

```text
ID:   0x04F02470
Data: D2 04 19 00 03 04 08 0C
```

Expected decoded values:

```text
steering DCAC system fault code       1234
braking DCAC system fault code          25
drive MCU system fault code              3
stir MCU system fault code               4
suction head MCU system fault code       8
front conveyor MCU system fault code    12
fault total                               6
```

For the current six signals, the page is called with:

```c
frmpage(ControllerFaultFrame, ControllerFaultFrameUpdate, 1);
```

If more fault signals are added later, follow the original project's direct paging style:

```c
frmpage(ControllerFaultFrame, ControllerFaultFrameUpdate, 2);
```

Then use the existing `index` argument to draw page 1 or page 2. Do not introduce generic page-count macros, helper layers, or a new paging framework unless the actual requirement needs them.

## Font Coverage Pitfall

The Chinese display fonts are generated subset fonts, not complete Chinese fonts. Before adding new Chinese UI text, verify that every character exists in the selected font source, such as:

```text
02-Code\app\gfx_apps\simple_draw\src\font\wryh31.c
```

`zk_printZH()` stops drawing the rest of a string when `zk_getCharInfo()` cannot find a character. This can look like a CAN parsing or layout problem even when the numeric value is correct.

This occurred on the controller fault page:

- The font did not contain `走`, so `行走驱动系统故障码` displayed only `行`.
- The font did not contain `搅`, so `搅拌MCU系统故障码` displayed no description.
- The numeric fault values `3` and `4` still displayed correctly, proving the CAN data and row calculation were working.

The current display text uses characters already supported by `GUI_Fontwryh31`:

```text
行车驱动系统故障码
混料MCU系统故障码
```

If the protocol wording must be displayed exactly, regenerate the font source with all required characters. Do not manually edit generated bitmap arrays in `wryh31.c`.

When a Chinese string is partially or completely missing but the value beside it is correct, check font coverage before changing CAN decoding, fault filtering, or row positioning.

## Guidance Quality Requirements

When proposing project changes:

- Read the relevant existing implementation first.
- Follow the original project's direct C style and existing page/CAN patterns.
- Avoid adding abstractions, helper layers, generic macros, or frameworks that the original project does not use.
- Do not solve a six-item, one-page requirement with a generic paging design before more pages actually exist.
- State the exact file, function, nearby code, and insertion or replacement position.
- Do not give vague instructions such as only saying "add this", "replace this", or "put it nearby".
- For every proposed edit, provide a searchable anchor from the current code and say whether the new code goes before it, after it, or replaces the whole function/block.
- When several related edits are required, list every affected location separately, including enum values, menu arrays, loop counts, cursor boundaries, key mappings, page dispatch, CAN software tables, hardware filters, and filter counts.
- Provide complete directly pasteable code for a new function or for any block that is safer to replace as a whole. Do not provide only disconnected fragments when their placement is ambiguous.
- Base line references and code anchors on the user's current working tree, not an older version previously discussed.
- For review requests, inspect the user's current code instead of repeating an earlier template.
- Clearly distinguish among a proposed change, a change visible in the current files, a successful build, and a bench-tested behavior.
- When values display correctly but labels do not, investigate font coverage and rendering behavior before blaming CAN reception.

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
