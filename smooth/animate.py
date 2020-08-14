# This script executes "animate.c" (compiled as "a.out") and creates 
# "output.mp4" using the c program's output and ffmpeg.

numframes = input('Number of frames: ')
numsteps = input('Number of operations between frames: ')
framerate = input('Animation framerate: ')

from os import system

system('rm output.mp4')
system('rm out???.pgm')
system('echo ' + numframes + ' ' + numsteps + ' | ' + './a.out')
system('ffmpeg -framerate ' + framerate + ' -i out%03d.pgm output.mp4')
system('rm out???.pgm')

