DSO203 GCC v1.27 APP - Community Edition
-------------------

Started out as just some fixes over Marco Sinatti's 1.8 version of the DSO203 APP plus GCC compilation support.
Hopefully, the users community can get involved and contribute more fixes and enhancements.

(Win32 GCC support by gabonator1)
 
Tested with:
	- SYS 1.50 1.6 from Marco Sinatti ( http://pmos69.net/dso203/SYS25116.hex )
	(******* NEEDED FOR CORRECT GENERATOR FREQUENCIES *******)
	
	- FPGA 2.61 ( http://pmos69.net/dso203/FPGA261.zip )
	- HW 2.6

PS: Do a calibration after changing firmware.
	
Please contribute or simply enjoy.

Pedro Simões

-------------------------------------------

Main button functions:

- Button 1 (>||)
	- Short press:			Toggle Run/Hold status
	- Long press: (1/2s)		Shortcut to channels A or B volts/div: will move mode selection to last selected channel. If already on a selected
                                        	channel's volts/div, will move selection to the other (A or B) channel's volts/div; if the channel is off, will turn it on.
- Button 2 (square)
	- Short press           	Select auto trigger level preset, will default to 1/2, then > 3/4 > 1/4
	- Long press: (1/2s)       	Selects manual trigger level, moves selection to trigger level control
	- Extra long press (3s):    	Calibration screen    

- Button 3 (circle)
	- Short press:              	Toggles Meters on/off
	- Long press:(1-1/2s)         	Save settings

- Button 4 (triangle)
	- Short press:                	Change between menu items   (previously @ left toggle center press... much easier to press this often used control)
	- Long press:(1/2s)          	Shortcut to time base time/div     

- Toggle buttons:
	- Left toggle left/right:     	Change menu item settings
	- Left toggle center press
		- Short press:         	Sets triggering to presently selected channel (eg: after turning a channel on with toggle, press to set triggering to this ch)
		- Long press:(1/2s):   	-With meters ON: Change between meter presets    (previously button 4 short press)
                                       	-With meters OFF: Toggles waveform gain calibration ON/OFF.  OFF= improves waveform quality, eliminates visible 
                                        	steps introduced to adjust gain. ON= corrects waveform gain (if needed) at the expense of waveform quality. 
                                        	Does not affect meters, gain always corrected for meters.  Menu displays "C" for calibration ON and "Un" for OFF.

	- Right toggle left/right:    	Change menus
	- Right toggle center press
		- Short press:        	Change between full buffer mode and single screen buffer mode    (previously button 4 long press)
		- Long press:(1/2s)    	Change between menu selection and meter selection    (previously short press) 

- FFT function (channels A or B) available in the channel 4 menu.
- Simple Spectrogram (channels A or B) available in the channel 4 menu.

-------------------------------------------

Access source in github:
  https://github.com/pmos69/dso203_gcc 

or get a zip with the complete tree here:
  https://github.com/pmos69/dso203_gcc/zipball/master
  
To build, simply edit makefile.bat to reflect the location of your Arm toolchain and the windows drive letter of the DSO.
The App will be built and copied to the DSO (first connect and boot the DSO in upgrade mode - turn on while pressing Button 1 >|| )

If you just want the APP, get it here: http://pmos69.net/dso203/APP_G251.hex

-------------------------------------------

Uses CodeSourcery Arm toolchain:
  https://sourcery.mentor.com/sgpp/lite/arm/portal/release1802
Just download the Windows TAR archive and unpack it somewhere, no installation is required.
Watch out for symlinks in the TAR you'll have to re-create them or copy/rename files to "fill the gaps"

-------------------------------------------

All thanks to:
- The original firmware developers
- Marco Sinatti (marcosin)
- Gabriel Valky (gabonator1)
- JackTheVendicator
- Jerson (http://jerson.co.in)
- vblack
- dementianati
- Wildcat

-------------------------------------------

Revisions:

v1.27
- FFT (channel A or Channel B) and Spectrograms  (channel A or Channel B) now accessible in the Channel 4 menu
- Merged all contributions from Wildcat:
	INTERFACE:
		- Variable repeat rate for toggles, slow for menus, fast for verniers, etc.
		- Main Menu will stop at left (CH-A), and at right on Time Vernier or Volume adj, will not "go around".
		- Program will attempt to set triggering to relevant channel, if shutting off a channel with trigg set, 
  			will move trigger source to last selected ch. 
		- Will not time out into standby if USB power plugged in.
		- "SCAN" mode renamed "TrOFF". Trigger levels/vernier do not show in this mode. 
		- "NorCL" (Normal-Clear) mode added (simulates regular CRT type scope, clears display if no trigger)
		- Single shot trigg mode automatically goes into full size buffer mode.
		- Digital channels meter default displays changed to show only relevant meters (no volt meters)
		- Trigg verniers disabled for digital channels (preset at 1/2)
		- X_Y-A and X_Y-S removed, now only X_Y. Mode is triggered in AUTO mode to allow meters to work correctly. (Move trigg 
  			source to unused ch to disable triggering if desired).
	TRIGGERING:
		- Re-sequencing of triggering functions to allow proper time for reset to bring up start flag before attempting to read
  			FIFO, allows analog channels to trigger properly on slow/random signals at fastest timebases.
		- Fixed a problem in the mid-range time bases where display would freeze or trigger very sporadically 
		- Implementation of proper triggering in single screen buffer mode.
		- Various functions optimizing triggering for different timebases, triggering modes, etc.
		- "SCAN" mode (now renamed "TrOFF") optimized with different routines for fast and slow time bases.
		- Single screen buffer mode now resets immediately after right edge of screen, also is available at all time bases.
		- Auto trigger fix to eliminate condition in some cases where device could not trigger because trigg level was too far 
  			away from wave and auto function could not find wave because device was not triggering.
	CALIBRATION:
		- Gain calibration now modifies signal in relation to signal zero point, rather than screen bottom. Prevents
  			interfering with offset calibration and Y positioning.
		- Signed variables now used to allow values at bottom of screen, which can be pushed up by offset calibration, and
  			then be below zero, to be read properly.
		- ADC operating point shifted away from non-linearity at the bottom of screen. Allows calibration routine to 
  			work more accurately.
		- Fix for an error in FPGA where the 2 least significant bits of CH-B are swapped.  
		- Fix for an error in saving parameters to disk where CH-B offset compensation could be corrupted if CH-A's value was 
  			negative. 
		- Signal gain calibration now switchable ON/OFF (Screen only, does not affect meters. Improves wave quality by removing
  			correction "steps", if present).
	FILES:
		- Fixed BMP header. Header was missing a row of digits causing improper file and image size to be listed.
		- Added several additional parameters to be saved in "save config". (Config file still compatible with previous versions)
	METERS:
		- Values now corrected by calibration in reference to signal zero point, rather than screen bottom. Prevents offsets and 
  			Y positioning from affecting accuracy.
		- Time measurements in process completely re-written, eliminating the inclusion of partial waves before triggering. While 
  			this only affected accuracy slightly in full frame buffer mode, it introduced major inaccuracies in single screen buffer 
  			mode where at times only a few waveforms are considered in the measurements.
		- Frequency meter fixed so it works past ~2Mhz. This worked OK in original program, but had wrong suffix. Subsequent fix for
  			improper readout caused meter to malfunction past around 2Mhz.
		- Fixed Time and Voltage Vernier meter readouts so they display properly at very large and very small time/voltage settings.
	GENERATOR:
		- Shifting of amplitude variation from signal's zero point rather than screen bottom, eliminates distortion at lower levels
  			where bottom of wave is clipped. Note that this results in an unchanging DC offset as level is varied... (use AC coupling
  			to display properly)
		- Changed division ratios so proper frequency is produced/displayed without needing a special SYS version (should work OK with
  			it as well) Only issue with this is the innability to produce 20Khz (produces 25Khz instead) Readout shows correct value.
	DISPLAY:
		- Improved linearity from ADC operating point shift. (Eliminates compression of wave at bottom of screen).
		- Bit swap correction on CH-B improves trace quality.
		- Elimination of noise at beginning and end of trace on some ranges.
		- Alignment of Digital channels with Analogs. Note that digital channels trailing at very fast timebases is caused by
  			delay introduced in Digital channel input stage. There is no way to fix this at the software level, needs to be fixed 
  			in hardware.  This same input stage configuration also introduces instabilities when triggering from the digital CHs at 
  			fast time bases.
		- Re-aligned trigger point with trigger vernier
		- Fixed "time shift" at fastest timebases when changing time/div while on hold.

v1.26
- readme changes

v1.25
- Patch for Process.c to correct problem with (A-B) calculation. (dementianati)

v1.24
- (Quick&Dirty) Fix for analog channels sampling delay in relation to the digital channels
	- 5 sample shift in the display of the analog channels when any digital channel is also visible - the first 5 samples are dropped from display
- Fix for the clipping of values exported to CVS
	- Now has full range of 0-255 instead of 0-200

v1.23
- Fix track color inconsistence for 'vertical' part of track (vblack)

v1.22
- FFT flickering fix, with improved drawing code (vblack)

v1.21
- SGL Trigger fix (JackTheVendicator)

v1.20
- Improvements in the FFT (Marco Sinatti):
	- The FFT function and FFT source can be selected from the track4 menu, it's possible to select the ch A or ch B.
		They are displayed as track4 math results.
	- Changed some colors
	- Changed the FFT meter positions, the max meter moves along the peak position
- Trigger fixes (JackTheVendicator)
- Fixed volume control + selection bounce (Jerson)

v1.19
- Fixed incorrect Nyquist in FFT at higher sample rates - Now uses the timebase interpolation coefficient.
- Fixed Vdc overflow bug introduced in v1.18
- Simple Spectrogram (Test version) - Select it from in the trigger mode/X_Y menu.

v1.18
- More FFT fixes
		- Better timming (less flicker)
		- Drawn over the curve (instead of merge)
		- Accurate scalling (was of by one bit)
		- All valid FFT buckets now displayed (bucket 1 is back from a vacation)
		- Peak frequency marker (chip ripoff)
		- Show FFT meters in the upper right corner:
			- Nyquist (max range)
			- Max: Predominant frequency in the sample
			- Div: Frequency range per divison
			- T1 : Frequecy at marker T1

v1.17
- Complete FFT re-implementation: (still has bugs)
		- FFT assembly routines replaced with "no nonsense" integer C code
		- All calculations made in-place (half the memory used)
		- Now using 512 samples (256 buckets)
		- FFT range boundary painted in blue
- Project cleanup (code size reduction)

v1.16
- Replaced Hanning window lookup table with function (memory optimization)

v1.15
- Small optimizations in some FFT operations
- Fixed FFT max frequency and scale max frequency display for time scales less than 10uS/Div

v1.14
- More (clueless) FFT tweakings
		Replaced the Hanning Window
		Applied dumb automatic scaling to the bars (based on max frequency)
		Removed "bluring"

v1.13
- Corrections in the FFT calculations
		(was making a mess, as usual)
		
v1.12
- Display of FFT data (over the buffer position box)
		on the left: Peak frequency detected in the FFT
		on the right: The max frequency of the FFT graph scale
- Fixed trigger level display (Thanks Marco)

v1.11
- Corrections in TH and TL readings:
		TH calculations for channels B, C and D were wrong.
		TH and TL readings were swapped for all channels.
- Fixed Frequency unit for K and M
		was: Hz - KC - MC
		now: Hz - KHz - MHz

v1.10
- Improved(/fixed) FFT calculation and display.

v1.9
- (crude) FFT spectrum analyzer (optimized Arm FFT code by Ivan Mellen)
		256 points
		channel A
		activate/deactivate with long press in button 1 (>||)
- corrections in RMS, Vpp, Min and Max (Vpp calculation wasn't consistent with min and max)
		added noise threshold to Min and Max
		adjusted noise threshold of fRMS
- no output option for generator
- Correct representation of the XPOS box relative size (view window size <-> capture buffer size)

v1.8
- Fixed horizontal display (previous blunder with X_SIZE)
- Minor code clean up (and code size reduction)

v1.7
- Fixed x10 probe modes - Division scales and meter values
- Calibration accuracy fix

v1.6
- Fixed X_Y modes - now operational again
- Fixed Min, Max and Vpp readings (initialization values)

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
	 

-------------------------------------------

Marco Sinatti (marcosin) Revisions:
	 
Versione APP251_V1.0 Richiede FPGA V2.22 - SYS1.50
-Tasto quadrato = Abilita il buffer ridotto a una schermata, disabilita lo scorrimento
-Tasto triangolo = cambio visualizzatori misure
-Corretto visualizzazione unità di misura della frequenza

Versione APP251_1.1 Richiede FPGA V2.22 - SYS1.50
-Aggiunto opzione portata x10 in modo da leggere la tensione corretta sui meter quando si utilizza la sonda x10
-Aggiunto accesso veloce ai cursoti V1 V2 T1 T2 in modo da poter andare direttamente a fare misure senza scorrere tutti i menù. Premendo il tasto si accede a V1, ripremendolo a V2, etc... dopo T2 si torna alla posizione da cui si era partiti
-Aggiunto livello trigger automatico, si setta da solo al centro della forma d'onda, il tasto è comodo anche per accedere al volo al menù trigger
-Aggiunto indicatore della batteria in carica, mentre stà caricando il riempimento della batteria scorre
-Tolto la modalità NONE sulla base dei tempi, perchè era identica a SCAN
-Modificato l'indicazione dell'impostazione a frame singolo, adesso viene evidenziata con un rettandolo sull'indicatore del buffer in basso
-Migliorata l'acquisizione a frame singolo che lavorava male in alcune portate

Versione APP251_1.3 Richiede FPGA V2.22 - SYS1.50
-Visualizzazione a schermo intero nascondendo i meter
-Durante l'uso dei cursori, se i meter sono nascosti appare comunque il DeltaT e il DeltaV
-Risolto bug sulla visualizzazione della linea che appare sui primi pixel dello schermo
-Risolto bug aggiornamento livello trigger automatico
-Nella modalità trigger automatica si può decidere di impostare in automatico il livello a 1/2 1/4 3/4 della Vpp
-Modificato visualizzazione carica batteria con scorrimento quando effettivamente è in carica
-Aumentato velocità di scorrimento dei cursori di misurazione
-Modificato modalità di riduzione del buffer, adesso quando siamo in "frame singolo" in realtà lavora così:
SCAN = cattura e visualizza un solo frame, purtroppo non è un roll vero
NON SCAN = cattura 1,6 frame, in questo modo il trigger funziona bene, si ha la possibilità di scorrere la visualizzazione
-In modalità SCAN non si hanno più deformazioni della forma d'onda al centro dello schermo
-In modalità SCAN tolgo il riferimento della XPOS perchè non serve
-Aggiunto modalità PWM e regolazione ampiezza segnale in uscita, modificando il generatore di funzioni in questo modo:
SQUARE = Onda quadra da 0Hz-20Khz con ampiezza regolabile da 0 a 2,6V
TRIANG = Onda triangolare da 0Hz-20Khz con ampiezza regolabile da 0 a 2,6V
SEW = Onda a dente di sega da 0Hz-20Khz con ampiezza regolabile da 0 a 2,6V
SINUS = Onda sinusoidale da 0Hz-20Khz con ampiezza regolabile da 0 a 2,6V
PWM = Segnale PWM con duty regolabile da 0 a 100 %, frequenza da 10Hz a 8Mhz.

Versione APP251_1.4 Richiede FPGA V2.22 - SYS1.50_1.6 
-Funzione X_Y (da terminare)
-Cambio rapido menù meter modificato, adesso con i canali C e D ad OFF quando si cambia i meter non li imposta sui canali disabilitati
-Sistemato SYS per il problema del range tensione
-Sistemato SYS sul menù calibrazione
-Rimodificato singolo frame, quando impostato non fa scorrere XPOS

Versione APP251_1.5 Richiede FPGA V2.22 - SYS1.50_1.6 
-Corretto visulizzazione DeltaV quando X10 sul canale B (prendeva sempre il canale A)

Versione APP251_1.6  Richiede FPGA V2.22 - SYS150_1.6 
-Modalità SCAN a singolo frame in vero ROLL
-Aggiunto 1-2-5Hz su generatore
-Portato frequenza massima generatore a 50Khz 
-Aumentato da 36 a 72 campionamenti la generazione delle forme d'onda

Versione APP251_1.7  Richiede FPGA V2.22 - SYS150_1.6 
-Invertito titolo generatore e tempo base
-Spostato meter duty e tensione generatore
-Implementato XY_S (scan) XY_A  (auto) da terminare
-NORMAL migliorato, non cancella lo schermo quando attende un nuovo trigger

Versione APP251_1.8  Richiede FPGA V2.22 - SYS150_1.6 
-Migliorato griglia e sistemato scala su modalità XY