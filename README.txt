Just some kick&dirty fixes over marcosin 1.8 version of the DSO203 APP.

Win32 GCC support by gabonator1

-------------------------------------------

Access source in github:
  https://github.com/pmos69/dso203_gcc 

or get a zip with the complete tree here:
  https://github.com/pmos69/dso203_gcc/zipball/master

-------------------------------------------

Uses CodeSourcery Arm toolchain:
  https://sourcery.mentor.com/sgpp/lite/arm/portal/release1802
Just download the Windows TAR archive and unpack it somewhere, no installation is required.
Watch out for symlinks in the TAR you'll have to re-create them or copy/rename files to "fill the gaps"

-------------------------------------------

All thanks to:
- Seeed-Studio
- Marco Sinatti (marcosin)
- Gabriel Valky (gabonator1)

-------------------------------------------


v1.5

- Re-did a complete code merge of marcosin 1.8 & fixes into 2.51 by hand. - Now compiles with -Os
- X_Y modes don't work, and mess the buffer - Reset needed to proceed after using X_Y.


v1.4

- changed compile options: "-O3" replaced by "-O0 -fno-common -fzero-initialized-in-bss"
- removed explicit initialization of the FrameMode variable


v1.3

- initialized FrameMode variable at creation
	Can now start correctly everytime. (at least on my DSO)
	If you have problems with bogus display in full-buffer mode, try saving settings and restarting.


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