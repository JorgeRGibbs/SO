#cd "/mnt/c/Users/Jorge/Desktop"
#python3 practica1.py
#1.0    Started to spawn 3 children on the 2nd level
#1.1    Succesfully spawned 3 children with 3 children of their own from level 2
#1.3	Started Implementing multiple process levels
#1.4
import os
from multiprocessing import Process
jobs=[]

def parent():
	n = int(input("Número de niveles"))
	print("Parent: ",os.getpid())  #Vater
	for i in range(2):
			p = Process(target=child, args=(n,i))
			jobs.append(p)
			jobs[i].start()
	if i == 1:
		for j in range(2):
			jobs[i].join()

def child(n,it):
	if n>0:
		print ("Proceso Hijo PID: ",os.getpid()," PPID: ",os.getppid())
		arr=[]
		if it==0:
			#print("Drei")
			for i in range(3):
				c = Process(target=child, args=(n-1,it))
				arr.append(c)
				arr[i].start()
				if i == 3:
					for j in range(3): #waits
						jobs[j].join()
		else:
			#print("Vier")
			for i in range(4):
				c = Process(target=child, args=(n-1,it))
				arr.append(c)
				arr[i].start()
				if i == 4:
					for j in range(0,4):
						jobs[j].join()


if  __name__ == '__main__':
	#print("Prozesse Baum")
	print("Arbol de procesos")
	parent()

