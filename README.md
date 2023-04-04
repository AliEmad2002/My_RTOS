# My_RTOS
ROTS implementation for ARM Cortex-M3 (Learning purposes)

* Features:
  - Thread stack is statically allocated and of configurable size.
  - Configurable maximum number of priorities.
  - Contex switching, with stacking all core registers which are not in the arm defined stack frame.
  - Ready/Waiting tasks are queued, resulting in minimum searching time and achiveing round-robin balancing.
  - Sleep on idle, to save power (demonstrated below).
  
* Current consumtion of the demo main.c (blinking two LED's at different frequencies, F_sys = 72MHz):
  - with sleep on idle enabled: (about 9.5mA)
 
https://user-images.githubusercontent.com/99054912/227711136-2106d95a-e569-4d67-ab02-f891d2264164.mp4
  - without sleep on idle enabled: (about 15mA)
 
https://user-images.githubusercontent.com/99054912/227711220-4dcb2d61-02d2-4054-a1c9-e5da78ef3deb.mp4





