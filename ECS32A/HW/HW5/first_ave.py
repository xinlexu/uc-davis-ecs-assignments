# first_ave.py
#
# calculate the average of the k years before, the year itself, and the k years after. 

#empty list of value
temps = []

#get file name and k from user
filename = input("Temperature anomaly filename:")
k = int(input("Enter window size:"))
infile = open(filename)

#ignore the header
infile.readline()

for line in infile:
    
    #strip whitespace off ends
    line = line.strip()
    
    #extract in variables
    year,value = line.split(",")
    
    #append number to list
    temps.append(float(value))

#compute the moving average for the first valid index in a list of temperature 
index = k
year = 1880 + index
ave = sum(temps[0:k+k+1]) / (2*k+1)

#formatted average temperature with 4 decimals
ave = "{:.4f}".format(ave)

#output the program
print(str(year)+","+str(ave))
infile.close()
