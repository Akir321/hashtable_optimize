import matplotlib.pyplot as plt
import csv


title     = "GNU hash"

fileName  = "hashGNU.csv"
legend    = ["GNU hash"]

dataDir   = "hash_data/"
graphDir  = "hash_data/"

X = []
Y = []


with open(dataDir + fileName, 'r') as dataFile:
    plotting = csv.reader(dataFile, delimiter=',')
        
    for ROWS in plotting:
        X.append(float(ROWS[0]))
        Y.append(float(ROWS[1]))

plt.bar(X, Y)
X.clear()
Y.clear()

#print("x[0] = " + X[0])
#print("y[0] = " + Y[0])

plt.legend(legend)

plt.title(title)

plt.xlabel("list number")
plt.ylabel("amount of keys")

plt.savefig(graphDir + title)
plt.show()


