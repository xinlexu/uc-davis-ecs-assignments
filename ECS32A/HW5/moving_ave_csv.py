# moving_ave_csv.py
#
#writes the values to an output file that includes a simple one line header.

#empty list of value
temps = []

#get file name and k from user
filename = input("Temperature anomaly filename:")
k = int(input("Enter window size:"))
infile = open(filename)

#open file to write
outfile = open("MovingAve.csv","w")
outfile.write("Year,Value\n")#column header

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

        #write on CSV file
        outfile.write(str(year)+","+str(ave)+"\n")
        
infile.close()
outfile.close()
                            
