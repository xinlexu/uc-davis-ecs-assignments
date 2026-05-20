# read_temp_file.py
#
# open the file from user for reading

#get file name from user
filename = input("Temperature anomaly filename:")
infile = open(filename,"r")
infile.readline() #ignore the header
for line in infile:
    #strip whitespace off ends
    line = line.strip()
    #extract in variables
    year,value = line.split(",")
    print(year,float(value))
infile.close()
