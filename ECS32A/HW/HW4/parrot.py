# parrot.py
#
# No matter what the input is, the program output will be in uppercase.
# Enter "hush" to exit

#loop until "hush" to stop
while True:
    user_input = input(">")
    #convert input to upper case
    user_input = user_input.upper()
    #hush to exit
    if user_input != "HUSH":
        print(user_input)
    else:
        break
