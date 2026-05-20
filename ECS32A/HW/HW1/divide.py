#divide.py


number_1 = int(input("Enter a number:"))
number_2 = int(input("Enter a number to divide that by:"))
quotient = int((number_1 / number_2))
remainder = int(number_1 - (quotient * number_2))
print(str(number_1)+" divided by "+str(number_2)+" is "+str(quotient)+" with "+str(remainder)+" remaining")
