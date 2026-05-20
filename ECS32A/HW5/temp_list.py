# temp_list.py
#
# Comment out the line that prints out the year and temperature for each year in the file

#empty list of value
temp = []

#get file name from user
filename = input("Temperature anomaly filename:")
infile = open(filename,"r")

#ignore the header
infile.readline()

for line in infile:
    
    #strip whitespace off ends
    line = line.strip()
    
    #extract in variables
    year,value = line.split(",")
    
    #append number to list
    temp.append(float(value))
    
print(temp)
infile.close()
