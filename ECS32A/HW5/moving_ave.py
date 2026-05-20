# moving_ave.py
#
# calculate the moving average of the k years before and k years after 

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

i = k
while i < len(temps)-i:

    # loop slides the window from index k to len(temps) - 1 - k
    # for each index we calculate the corresponding year and 
    # the average of the elements from temps[index-k] to temps[index+k] inclusive
    for index in range (k,len(temps)-k):
    
        # calculate year from index
        year = 1880 + index

        # calculate average for the window centered at index
        ave = sum(temps[index-k:index+k+1]) / (2*k+1)

        #formatted average temperature with 4 decimals
        ave = "{:.4f}".format(ave)

        i = i+1

        #output the program
        print(str(year)+","+str(ave))
infile.close()
