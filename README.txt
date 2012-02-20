Just some kick&dirty fixes over marcosin 1.8 version of the DSO203 APP.

Win32 GCC support by gabonator1

Uses CodeSourcery Arm toolchain ( https://sourcery.mentor.com/sgpp/lite/arm/portal/release1802 )

-------------------------------------------

All thanks to:
- Seeed-Studio
- Marco Sinatti (marcosin)
- Gabriel (gabonator1)

-------------------------------------------


v1.2:

- automatic chinese->english translation of all chinese comments in the source.


v1.1:

- Updated with marcosin 1.8 release


v1:

- Removed -fno-common and changes -Os to -O3 in compile options
     APP still only starts correctly 1/3 of the times

- Fixed Vdc and RMS values display when in 1-page buffer mode (menu.c)
     Calculation didn't take into account the buffer size when calculating the averages

- Fixed wrong calculations for Vdc and RMS (process.c)
     the counters (a_Avg, b_Avg, a_Ssq, b_Ssq) were initialized with a value different than 0)