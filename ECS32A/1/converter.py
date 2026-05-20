#converter.py


character = input("Enter a character:")
num = ord(character)
binary = bin(num)
print(str(character)+(" corresponds to the integer ")+str(num)+(" which is ")+str(binary)+(" in binary."))

