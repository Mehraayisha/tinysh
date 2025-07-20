#!/usr/bin/env python3
import curses
from curses import KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, wrapper
from random import randint
import time

def main(stdscr):
	height,width=stdscr.getmaxyx()#get max height and width of screen
	stdscr.border()#border
	stdscr.timeout(75)
	segments=[[1,1]]
	stdscr.addch(*segments[0],curses.ACS_BOARD)
	dirs={#y,x ord return the unicode of the letter
	    ord("w"):[-1,0],#up
		ord("s"):[1,0],#down
	    ord("a"):[0,-1],#upper y axis part is -1
	    ord("d"):[0,1]
	}
	direction=[0,1]#initialise to right
	while True:
		key = stdscr.getch()
		if key in dirs:
			direction=dirs[key]

		head=[segments[0][0]+direction[0],segments[0][1]+direction[1]]#add the direction coordinate to the head postion to get new position of head
		if head[0] in [0,height-1] or head[1] in [0,width-1]:#if head goes out of border exit
			break;
		segments.insert(0,head)#add head in new position
		stdscr.addch(*segments[0],curses.ACS_BOARD)#draw the new head to screen

		tail=segments.pop()#delete the old tail
		stdscr.addch(*tail,' ')#clear the old tail from the screen
		

wrapper(main)
