compile:
	gcc src/main.c -o main `pkg-config --cflags --libs gtk+-3.0`
