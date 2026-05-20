# cash2.py
# Cash register
# able to remove any leading dollar sign characters form the input and skip over lines of inout that can not be converted to floating point numbers.


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
    #remove the dollar sign
    line = line.strip('$')
    #try to convert line to float
    try:
        
        num = float(line)
        item = item + 1
        total = total + num
    #fail to convert line to float
    except:
        continue
#formatted totaling
total = ("${:.2f}".format(total))

print("File contained",item,"items totaling",total)
