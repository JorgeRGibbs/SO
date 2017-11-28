import os, sys

def leftChild(count, level):
    r,w = os.pipe()
    for it in range(3):
        idprocess=os.fork()
        if idprocess == 0:
            print "NIVEL["+str(count)+"] Yo soy ("+str(os.getpid())+") hijo izquierdo de\t <- "+str(os.getppid())+" -|\n"
            leftChild(count+1,level)
            break
        elif idprocess>0:
            if it==2:
                os.wait()
    return

def rightChild(count, level):
    if count == level:
        sys.exit(0)
    for it in range(4):
        idprocess=os.fork()
        if idprocess==0:
            print "NIVEL["+str(count)+"] Yo soy ("+str(os.getpid())+") hijo derecho de\t <- "+str(os.getppid())+" -|\n"
            rightChild(count+1,level)
            break
        elif idprocess>0:
            if it==3:
                os.wait()
    return

def main():
    count = 0
    level = input('Ingrese el numero de niveles: ')
    while level<=0:
        level = raw_input('No se pueden tener 0 o menos niveles, ingrese numero de niveles: ')
    if level == 1:
        print "NIVEL["+str(count)+"] Yo soy satan ("+str(os.getpid())+") hijo de\t <- "+str(os.getppid())+" -|\n"
        return
    print "NIVEL["+str(count)+"] Yo soy satan("+str(os.getpid())+") hijo de\t <- "+str(os.getppid())+" -|\n"
    for it in range(2):
        idprocess=os.fork()
        if idprocess==0:
            if(it==0):
                print "NIVEL["+str(count+1)+"] Yo soy ("+str(os.getpid())+") hijo izquierdo de satan\t <- "+str(os.getppid())+" -|\n"
                leftChild(count+2, level)
                break
            if(it==1):
                print "NIVEL["+str(count+1)+"] Yo soy ("+str(os.getpid())+") hijo derecho de satan\t <- "+str(os.getppid())+" -|\n"
                rightChild(count+2,level)
            break
        elif idprocess>0:
            if it==1:
                os.wait()
    os.exit(0)


if __name__ == "__main__":
    main()
