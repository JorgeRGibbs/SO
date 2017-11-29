from PIL import Image
import os 
from multiprocessing import Process,Pipe
import sys

#CHECKPOINT--------------------------------------------------------------------------------------
def Filter(conn,pixel,height,width,a,b):	
	for x in range(width):
		for y in range(height):
			pixels=list(pixel[x,y])
			pixels[a] = 0
			pixels[b] = 0
			pixel[x,y] = tuple(pixels)
	#c_conn.close()
	#img.show()
	#c_conn.send(pixel)
	
	#c_conn.close()
	return pixel,img

def child(conn,img,i,pixel):
	print("PID:",os.getpid(),"PPID",os.getppid())
	#create(width,height)

	if i == 0:
		Filter(conn,pixel,height,width,1,2) #R
		#img.show() #bild anzeigen
		c_end[2].send(img)
		print("Sent red.")
		c_end[2].close()
	elif i == 1:
		Filter(conn,pixel,height,width,0,2)#G
		#img.show() #bild anzeigen
		c_end[1].send(img)
		print("Sent green.")
		c_end[1].close()
	else:
		Filter(conn,pixel,height,width,0,1)#B
		c_end[0].send(img)
		print("Sent blue.")
		c_end[0].close()
		#img.show() #bild anzeigen
		#img.show()
	#c_conn.send(color)
	#c_conn.close()
	#conn.send([new])
	#conn.close()


if __name__ == '__main__':
	c_end= []
	p_end = []
	jobs = []

	print("1.- Ziggy Stardust")
	print("2.-The Warriors")
	print("3.-In Rainbows")
	opc = int(input("Escoge una imagen"))
	if opc == 1:
		img=Image.open("/home/valtzz/Desktop/ziggy.jpg")
	elif opc == 2: 
		img=Image.open("/home/valtzz/Desktop/warriors.jpeg")
	elif opc == 3:
		img=Image.open("/home/valtzz/Desktop/rainbows.jpg")
	else: 
		img=Image.open("/home/valtzz/Desktop/heheboi.jpg")

	print("PID:",os.getpid())
	width,height = img.size
	pixel = img.load()
	#img.show()
	
	for i in range(3):
		p_conn,c_conn = Pipe()
	#	greenin , greenout = Pipe() ACHTUNG!
		c_end.append(c_conn)
		p_end.append(p_conn)


	for i in range(3):
		c=Process(target=child, args=(c_end,img,i,pixel))
		jobs.append(c)
		jobs[i].start()
		if i == 0:
			red = p_end[2].recv()
			print("Recieved red.")
			p_end[2].close()
			#c_end[2].close() nein
			red.show()
			
			red.save('red.jpg')
			print("Saved red.")
			#img.close()  rot fertig.
		#	pixels[0] = p_end.recv(p_conn)
		#	p.close() 
		#	pixels[0].save(red.jpeg)
		if i == 1:
			green = p_end[1].recv()
			print("Recieved green.")
			p_end[1].close()
			#c_end[1].close()
			green.show()
			green.save('green.jpg')
			print("Saved green.")
			#img.close()
		#	pixels[1]= p_end.recv(p_conn)
		#	p_end.close()
		#	pixels[1].save(green.jpeg)
		#conn.close()
		if i == 2:
			blue = p_end[0].recv()
			print("Recieved blue.")
			p_end[0].close()
			#c_end[0].close()	
			blue.show()
			blue.save('blue.jpg')
			("Saved blue.")
			#img.close()
			#pixels[2] = p_end.recv(p_conn)
			#p_end.close()
			#pixels[2].save(blue.jpeg)
	for j in range(3):
		jobs[i].join()

