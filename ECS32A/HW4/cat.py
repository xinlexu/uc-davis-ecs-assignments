# cat.py
#
# open a filename provided by the user and print every line of the opened file to the screen without modification.

#input filename
file_name = input("Enter a file name to open:")

#file opened for reading
infile = open(file_name,"r")
for line in infile:
    #strip whitespace from ends
    line = line.strip()
    print(line)
