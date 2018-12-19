# parseBMPtoRGBArray
This program is for my Android projects.

It can parse the BMP images to RGB array and store to files for Android bootloader images format(splash).
1. 3 bytes per pixel
  0xaa, 0xbb, 0xcc, ... (R G B or B G R)
2. 2 bytes per pixel
  0xaa, 0xbb, ... 
3. Gray level, 1 bit per pixel

But I didn't use it for a long time. Maybe there is some bugs after I modified for pushing to github. 
