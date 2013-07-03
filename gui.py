#!/usr/bin/python

from Tkinter import *
import tkFileDialog

class App:

	def __init__(self, master):
		frame = Frame(master)
		frame.pack()

		self.exit_button = Button(frame, text="QUIT", fg="red", command=frame.quit)
		self.exit_button.pack(side=LEFT)

		self.open_button = Button(frame,text="Open", command=self.open_file)
		self.open_button.pack(side=LEFT)

	def open_file(self):
		file = tkFileDialog.askopenfile(parent=root,mode='rb',title='Choose a file')
		if file != None:
			data = file.read()
			file.close()
			print "I got %d bytes from this file." % len(data)

root = Tk()

app = App(root)

root.mainloop()
root.destroy()
