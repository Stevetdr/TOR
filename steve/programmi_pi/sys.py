
#!/bin/env python3.4
import sys
print (" per prova mettere piu' argomenti tipo: python3.4 sys.py pippo pluto paperino e dante")

print ("This is the name of the script: ", sys.argv[0])
print ("Number of arguments: ", len(sys.argv))
print ("The arguments are: " , str(sys.argv))
for x in range(0,len(sys.argv)):
    print ("The arguments nr. {0} is: {1}".format (x , str(sys.argv[x])))