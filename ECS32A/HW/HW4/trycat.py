# trycat.py
#
# keeps asking for the name of the file to open until it is opened.

#loop until file is opened
while True:
    try:
        #input filename
        file_name = input("Enter a file name to open:")

        #file opened for reading
        infile = open(file_name,"r")
        for line in infile:
            #strip whitespace from ends
            line = line.strip()
            print(line)
        break
    except:
        print("Could not open",file_name)
        continue
