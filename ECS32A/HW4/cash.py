# cash.py
#
# read the line from the file and convert the result to floating point number.

print("Cash register")
#prompt the user for the name of file to open
filename = input("Enter file of prices:")
#count item number
item = 0
#count item total
total = 0
#reading numbers from a file
infile = open(filename,"r")

# loop over each line of the input file
for line in infile:
    num = float(line)
    item = item + 1
    total = total + num
#formatted totaling
total = ("${:.2f}".format(total))

print("File contained",item,"items totaling",total)
