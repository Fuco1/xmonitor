all:
	gcc -o xmonitor xmonitor.c -lX11 -lXrandr -lm
