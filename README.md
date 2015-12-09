# Superpunto
SFML/Modern OpenGL clone of mrevenga's SDL punto ( http://punto.sourceforge.net/ )

#COMPILATION
Run the Makefile. You can define STATIC to include all the dependencies in the executable (-DSTATIC)


Dependencies:

1. SFML-2.0+ (2.3 recommended) and its dependencies. Only libsfml-graphics, libsfml-system and libsfml-window are required

2. OpenGL 3.0+ (3.3+ recommended)

You can grab a compiled version (**no need for SFML and its dependencies in your system**) in "releases".

Known to work in Ubuntu 14/15. You can install SFML by $ apt-get install libsfml-dev

You can also compile it yourself from https://github.com/SFML/SFML

#USAGE
Use with $ ./spunto inputfile [opts]

run ./spunto -h for information about the available options and controls.


inputfile should have the following structure:
>\#L=X; Comments are used to separate frames, you can force the size of the simulation box starting the comment with L=X; as in this example

>X1 Y1 Z1 r1 c1 #You can comment here aswell! If your file has more than
   
>X2 ...         #5 columns, the rest will be ignored!

>.

>.

>.

>\# frame=2

>X1 Y1 Z1 r1 c1

>.

>.

>.

>\# frame = 3

r1 is the size of the superpunto.

c1 is its color.

both of these parameters are optional, if you only set 4 columns, the 4th will be interpreted as the color.

If you generate a movie, use gif2mp4 to convert it to mp4.

#COLORS
The colors are RGB encoded as an int number varying from 0 to 255^3 = 16581375 as c = Decimal(0xRRGGBB)

Where RR/GG/BB go from 0x00=0 to 0xFF=255

i.e full red (255,0,0) would be 0xFF0000=16711680

#CONTROLS
You can move around using:

1. WASD for moving in your XY plane
2. LSHIFT and Lctrl to go up and down
3. E and Q to roll
4. Keep LAlt pressed to look around using the mouse
5. Move faster/slower using + -
6. Numbers 1-6 to rotate the system in the X, Y o Z axis 

You can go through time using:

1. Space/R to go to the next/previous frame
2. Press M to autoplay the frames at 30 FPS
3. Press T to go to the last frame

You can take screenshots and record some frames in a gif using:

1. Press L or start using --record to take a screenshot every frame, these will be converted to gif at exit. You can move around while recording
2. Press C to take a single screenshot.






