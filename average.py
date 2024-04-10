import csv
import math

fileName = "hashAlways1.csv"


dataDir = "hash_data/"

values = []

with open(dataDir + fileName, 'r') as dataFile:
    plotting = csv.reader(dataFile, delimiter=',')

    for ROWS in plotting:
        values.append(int(ROWS[1]))
    dataFile.close()

length  = len(values)
valSum  = sum(values)
average = valSum / (length)

stddev  = sum( (average - val)**2 for val in values)
stddev  = stddev / length 
stddev  = math.sqrt(stddev)

print(fileName)
print("stddev = " + str(stddev))

