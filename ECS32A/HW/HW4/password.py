# password.py
#
# ask the user to enter their password and print the length of the password. Then it will print a message for each of four possible properties of the password.

password = input("Enter password:")

print("Length:",len(password)) 
#loop over each letter in password
for letter in password:
    #check lower case
    if letter.islower():
        print("Has lower case")
        break
for letter in password:
    #check upper case
    if letter.isupper():
        print("Has upper case")
        break
for letter in password:
    #check digit
    if letter.isdigit():
        print("Has digit")
        break
for letter in password:
    #check special
    if letter in "!@#$%^&*()-=_+{}|[]¥:<>?;',./":
        print("Has special")
        break
