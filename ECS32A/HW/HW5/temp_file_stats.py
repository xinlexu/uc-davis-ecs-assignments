# temp_file_stats.py
#
# find minimum and maximum temperature for all of the years in the file and the years they occured in.

#track year with min and max temp
temp_min = None
min_year = None
temp_max = None
max_year = None

#open the file from user input
filename = input("Temperature anomaly filename:")
infile = open(filename,"r")

#ignore the first line
infile.readline()

#loop obtain lines from the file
for line in infile:

    #strip the whitespace
    line = line.strip()

    #spilit into year,temp
    year, temp = line.split(",")

    #convert temp into float 
    temp = float(temp)

    #update min temp and the year corresponding to the min
    if temp_min == None or temp < temp_min:
        temp_min = temp
        min_year = year

     #update max temp and the year corresponding to the max
    if temp_max == None or temp > temp_max:
        temp_max = temp
        max_year = year

#output items
print("Min temp:",temp_min,"in",min_year)
print("Max temp:",temp_max,"in",max_year)
