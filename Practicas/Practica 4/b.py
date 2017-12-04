from multiprocessing import Process,Array,Manager,Value,Queue
import threading
from ctypes import c_char_p
from threading import Thread,Semaphore
from random import randint,sample
import time
import os
import logging
import queue
import re
#Viele Wörterbücher
#das wird schwer
threads = []
sam = sample(range(0,4),4)

def worker(q,flag):
	cad=chargen(flag)
	q.put(cad)
	print(threading.current_thread().getName(),''+cad+'.')

def consumer(q):
	cad=q.get()
	print(threading.current_thread().getName(),''+cad+'.')
#fertig

def chargen(flag):
	li = []
	#for i in range(1000):
	r = sam[flag]
	for j in range(4):
		if r==0:
			for x in range(4):
				li.insert(x,'x')
		elif r==1:
			for x in range(4):
				li.insert(j,'y')
		elif r==2:
			for x in range(4):
				li.insert(j,'z')
		elif r ==3:
			for x in range(4):
				li.insert(j,'w')
	print(li)
	cad = ''.join(li)
	str(cad)
	cut=re.findall('....',cad)	
	print(cut)
	cuts=''.join(cut)
	#str(cut)
	return(cuts)
	
if __name__ == '__main__':
	

	q=queue.Queue(maxsize=5)

	for i in range(4):
		name = 'Productor'+str(i)+': '
		t=threading.Thread(name= name ,target=worker,args=(q,i,))
		threads.append(t)
		t.start()
	for j in range(4):
		name = 'Consumidor'+str(j)+': '
		t=threading.Thread(name = name ,target=consumer,args=(q,))
		threads.append(t)
		t.start()
