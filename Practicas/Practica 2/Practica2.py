import os
from random import randint
from multiprocessing import Process
A=[]
B=[]
C=[]


def Multi(A,B,C,AB,CB,n,res,g,FA):
	#print ("Kinder Prozess: PID ",os.getpid()," Mein vater ist: PPID ",os.getppid())
	for i in range(res):
		if g<FA:
			for j in range(AB):
				for k in range(CB):
					C[g][j] = C[g][j] + (A[g][k] * B[k][j])
			print("C[g]")
			print("Soy el proceso",os.getpid()," hice la fila ",g," de C")
			g=g+1

def Matrices(FA,AB,CB):
	A= [ [ randint(0,50) for i in range(FA) ] for j in range(AB) ]
	B= [ [ randint(0,50) for i in range(AB) ] for j in range(CB) ]
	C= [ [ 0 for i in range(FA) ] for j in range(CB) ]
	print("Matriz A:")
	print("")
	print(A)
	print("Matriz B:")
	print("")
	print(B)

def Procesos(A,B,C,FA,AB,CB,n):
	#div=FA/n#round this shit
	div=float(FA)/float(n)
	if div>int(div):	#redondeo
		res=int(div+1)
	else:
		res=int(div)
	print("procesos:",n," div:",res,)
	g=0

	for it in range(n):
		proceso = Process(target=Multi, args=(A,B,C,AB,CB,g,n,res,FA))
		proceso.start()
		g=g+res
			
	if g == n:
		for q in range(i):
			proceso.join()

			
if  __name__ == '__main__':
	print("Multiplicacion de Matrices con procesos")
	FA = int(input("Ingresa el numero de filas de la Matriz A "))
	AB = int(input("Ingresa el numero de columnas de la Matriz A y Columnas en B "))
	CB = int(input("Ingresa el numero de columnas de la Matriz B "))
	n = int(input("Cuantos procesos?"))

	Matrices(FA,AB,CB)
	Procesos(A,B,C,FA,AB,CB,n)
