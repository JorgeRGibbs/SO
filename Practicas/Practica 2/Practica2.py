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
			print(C[g])
			print("PID",os.getpid(),":",g)
			g=g+1

def Matrices(FA,AB,CB):
	'''
    #inicializar matrices
	A= [ [ randint(0,50) for i in range(filasA) ] for j in range(FC) ]
    B= [ [ randint(0,50) for i in range(FC) ] for j in range(colsB) ]
    C= [ [ 0 for i in range(filasA) ] for j in range(colsB) ]
	print("Matriz A:")
	print("")
	print(A)
	print("Matriz B:")
	print("")
	print(B)'''
	print("A:")
	print("")
	for i in range(FA):
		A.append([])
		for j in range(AB):
			r = randint(0,50)
			A[i].append(r)
		print (A[i])
	print("B:")
	print("")
	for i in range(AB):
		B.append([])
		for j in range(CB):
			r = randint(0,50)
			B[i].append(r)
		print (B[i])
	print("Generando C...")
	for i in range(FA):
		C.append([])
		for j in range(CB):
			C[i].append(0)

def Procesos(A,B,C,FA,AB,CB,n):
	g=0
	#div=FA/n#round this , can't be float
	div=FA/n
	res = round(div)
	print(" división:",res,)
	for it in range(n):
		proceso = Process(target=Multi, args=(A,B,C,AB,CB,g,n,res,FA))
		proceso.start()
		g=g+res
			
	if g == n:
		for q in range(i):
			proceso.join() #for

			
if  __name__ == '__main__':
	print("Multiplicacion de Matrices con procesos")
	FA = int(input("Numero de filas en A "))
	AB = int(input("Numero de columnas en A y filas en B "))
	CB = int(input("Numero de columnas en B "))
	n = int(input("Cuantos procesos?"))

	Matrices(FA,AB,CB)
	Procesos(A,B,C,FA,AB,CB,n)

