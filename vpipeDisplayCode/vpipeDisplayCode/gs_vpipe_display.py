############################################################
#
# Displays PPM frame from file.
#
# Reads 240 lines of 320 bytes for grayscale frame that is
# 320 columns x 240 rows.
#
############################################################

from Tkinter import *
from socket import *
from select import select

myHost = ''
myPort = 50007

sockobj = socket(AF_INET, SOCK_STREAM)
sockobj.bind((myHost, myPort))
sockobj.listen(5)
readsocks, connection = [], []
img = ''
frame = ''
remaining = 76800
buffer = 0

win = Tk()

def WaitForConnect():
	global connection
	global readsocks
	global sockobj
	connection, address = sockobj.accept()
	#sockobj.setblocking(0)
	readsocks.append(connection)
	print 'Vpipe Display server connected to by', address

def CheckForFrame():
	global connection
	global readsocks
	global img
	global frame
	global canvas
	global buffer
	global remaining
	#print 'Check for frame'

        # polling select call
	readables, writeables, exceptions = select(readsocks, [], [], 30)

	if(readables):

		slice = connection.recv(remaining)
		frame = frame + slice

		if(len(frame) < 76800):
			#print 'Got partial frame = ', len(frame)
			remaining = remaining - (len(slice))

		elif(len(frame) == 76800): # One PGM frame
			remaining = 76800

			if(buffer == 0):
				print "entered"
				myfile = open('gs_vpipe.pgm', 'wb')
				myfile.write('P5\n')
				myfile.write('#grayscale\n')
				myfile.write('320 240\n')
				myfile.write('255\n')
				myfile.write(frame)
				myfile.close()
				img = PhotoImage(file="gs_vpipe.pgm")
			
			else:
				myfile = open('vpipe_frame1.ppm', 'wb')
				myfile.write(frame)
				myfile.close()
				img = PhotoImage(file="vpipe_frame1.ppm")
				buffer=0

			# Avoid file temp buffers with in memory conversion
			# to the photo image format from PPM array
                        #img = PhotoImage(data=frame)

			canvas.create_image(0,0,image=img, anchor=NW)
			canvas.pack()
			canvas.update()
			win.update()
			frame = ''
		else:
			print 'ERROR: Got giant frame = ', len(frame)
			remaining = remaining - (len(frame) - 76800)
			frame = '' # discard giant

	# print 'End check for frame'
        # win.after_idle(CheckForFrame)
	# See note below on difference between after and after_idle
	win.after(30, CheckForFrame)
	
def CloseConnection():
	print 'Closing connection'
	connection.close()

# Main initialization of the display and check for frame services
#
print 'Waiting for connection'
WaitForConnect()

#img = PhotoImage(file="test_frame.ppm")
canvas=Canvas(win, width=320, height=240, bg='white')
#canvas.create_image(0,0,image=img, anchor=NW)
#canvas.pack()
#canvas.update()
win.update()

# The after_idle call-back method will use up to 100% of available CPU trying
# to keep up with the frame rate from the server.
#
# A bit safer is the after call with 30 milliseconds specified so that frames
# will only be checked for at a maximum rate of approximately 30x per second.
# Most frame sources will have 30 fps as a maximum send rate anyway, so this
# avoids polling that can kill the resources on some operating systems like
# windows.
#win.after_idle(CheckForFrame)
win.after(30, CheckForFrame)


print 'Entering main loop'
win.mainloop()
