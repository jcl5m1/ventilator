# Dataset explaination:

Data rows are printed from the Arduino at approximately 100Hz (minus serial port overhead, and few if statements).
Filename format is A_B_C.csv with parts defined below

A: as follows:
 * cpap - constant pressure
 * bipap - two pressure cycle at 16 cycles/min

B: spdX where X approximately maps to:
 * 60 ~= 10cm/H20
 * 90 ~= 18cm/H20
 * 120 ~= 32cm/H20

C: as follows:
 * open - not in mouth
 * breathing - in mouth, breathing
 * breathing_sync - in mouth, breathing in sync with blower cycle
 * breathing_async - in mouth, breathing out of sync with blower cycle
