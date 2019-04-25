#!/usr/bin/env python3

import subprocess,os,re
from tkinter import *
from tkinter.ttk import *

def console():
	cmd = subprocess.Popen("ssh admin@roborio-1425-frc.local \"tail -f -n0 /var/local/natinst/log/FRC_UserProgram.log\"", shell=True, stdout=subprocess.PIPE)
	for line in iter(cmd.stdout.readline, ""):
		yield line
	cmd.stdout.close()

"""
for l in console():
	if l.find("CLEAR_SCREEN")==0:
		os.system("clear")
	else:
		print(l,end="")
"""

known_beginnings=["mode: ", "panel:", "main: ", "out: "]
no_record_beginnings = ["in: Robot_inputs(", "CLEAR_SCREEN"]
in_beginnings = ["mode=", "now=", "ds_info:", "0:", "1:", "2:", "dio:"]
row_titles = ["mode", "panel", "main", "out",  "robot_mode", "now", "ds_info", "Joystick 0", "Joystick 1", "Joystick 2", "dio"]

assert len(known_beginnings) + len(in_beginnings) == len(row_titles)

top=Tk()
top.wm_title("Netconsole")
top.minsize(width=700, height=300)

menu = Menu(top)
top.config(menu=menu)

rows = 0
labels=[]
labeltexts=[]
textvars=[]

misc_begin_length = 5
misc_beginnings = []

def add_row(label, text):
	global rows, labels, textvars
	
	labeltexts.append(label)
	l = Label(top, text=label)
	l.grid(column=0, row=rows, sticky="NW")
	labels.append(l)
	
	textvars.append(StringVar())
	textvars[rows].set(text)
	t = Label(top, textvariable=textvars[rows], wraplength=600)
	t.grid(column=1, padx=20, row=rows, sticky="W")
	labels.append(t)

	rows += 1

def update_row(label, text):
	global labeltexts, textvars
	row = labeltexts.index(label)
	textvars[row].set(text)

def refill_rows():
	for i in range(len(known_beginnings)):
		add_row(row_titles[i], "")
	add_row("in", "")
	for i in range(len(known_beginnings), len(row_titles)):
		add_row("  " + row_titles[i], "")
	top.grid_columnconfigure(1, weight=1)
refill_rows()

def reset_rows():
	global rows, labels, labeltexts, textvars, misc_beginnings
	for label in labels:
		label.destroy();
	labels = []
	labeltexts = []
	textvars = []
	misc_beginnings = []
	rows = 0
	refill_rows()
menu.add_command(label="Clear", command=reset_rows)

stopped = False
def toggle_update():
	global menu, stopped
	stopped = not stopped
	if stopped:
		menu.entryconfig(2, label="Start")
	else:
		menu.entryconfig(2, label="Stop")
menu.add_command(label="Stop", command=toggle_update)

for l in console():
	l = l.strip()
	if not stopped and len(l) > 0:
		found = False
		for i in range(len(known_beginnings)):
			if l[:len(known_beginnings[i])].decode("utf-8") == known_beginnings[i]:
				update_row(row_titles[i], l[len(known_beginnings[i]):].decode("utf-8")) 
				found = True
				break
		if not found:
			for i in range(len(in_beginnings)):
				if l[:len(in_beginnings[i])].decode("utf-8") == in_beginnings[i]:
					update_row("  " + row_titles[i + len(known_beginnings)], l[len(in_beginnings[i]):].decode("utf-8"))
					found = True
					break
		if not found:
			for i in range(len(no_record_beginnings)):
				if l[:len(no_record_beginnings[i])].decode("utf-8") == no_record_beginnings[i]:
					found = True
					break
		if not found and len(l) >= misc_begin_length:
			l_begin = l[:misc_begin_length]
			if misc_beginnings.count(l_begin) > 0:
				update_row(l_begin, l)
			else:
				misc_beginnings.append(l_begin)
				add_row(l_begin, l)
	top.update()
