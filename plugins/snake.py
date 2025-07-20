#!/usr/bin/env python3
import curses
from curses import KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, wrapper
from random import randint
import time

def main(stdscr):
	curses.curs_set(0)
	curses.init_pair(1,curses.COLOR_RED,curses.COLOR_BLACK)
	height,width=stdscr.getmaxyx()#get max height and width of screen
	stdscr.border()#border
	stdscr.timeout(100)
	segments=[[1,1]]
	stdscr.addch(*segments[0],'0',curses.color_pair(1))
	food_pos=[randint(1,height-2),randint(1,width-2)]#generate food in the range
	stdscr.addch(*food_pos,"*")#add food to screen in the generated position
	score=0
	dirs={#y,x ord return the unicode of the letter
	    ord("w"):[-1,0],#up
		ord("s"):[1,0],#down
	    ord("a"):[0,-1],#upper y axis part is -1
	    ord("d"):[0,1]
	}
	direction=[0,1]#initialise to right
	while True:
		stdscr.addstr(0,width-11,f"Score:{score}")
		key = stdscr.getch()
		if key in dirs and [dirs[key][0]+direction[0],dirs[key][1]+direction[1]]!=[0,0]:#dirs[key][0] means first element -1 if key= [-1,0]
			direction=dirs[key]

		head=[segments[0][0]+direction[0],segments[0][1]+direction[1]]#add the direction coordinate to the head postion to get new position of head
		if head[0] in [0,height-1] or head[1] in [0,width-1] or head in segments:#if head goes out of border exit
			break;
		segments.insert(0,head)#add head in new position
		if len(segments)>1:
			stdscr.addch(*segments[1],'o',curses.color_pair(1))
		stdscr.addch(*segments[0],'0',curses.color_pair(1))#draw the new head to screen

		if head!=food_pos:#remove tail only if the snake didnt eat food
			tail=segments.pop()#delete the old tail
			stdscr.addch(*tail,' ')#clear the old tail from the screen
		else:#add new food if the snake eats the existing food
			food_pos=[randint(1,height-2),randint(1,width-2)]#generate food in the range
			while food_pos in segments:
				food_pos=[randint(1,height-2),randint(1,width-2)]
			stdscr.addch(*food_pos,"*")#add food to screen in the generated position
			score+=1
			

wrapper(main)
