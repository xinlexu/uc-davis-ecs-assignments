#change.py
#
#making change by using the fewest number of coins

#ask total amount from user in cents
user_input = int(input("Enter change:"))

#calculate number of quarter
num_quarter = user_input // 25

#calculater number of dimes for the rest of money
num_dimes = user_input % 25 // 10

#calculater number of nickels for the rest of money
num_nickels = user_input % 25 % 10 // 5

#calculater number of nickels for the rest of money
num_pennies = user_input % 25 % 10 % 5

#output result
print("Quarters:",num_quarter)
print("Dimes:",num_dimes)
print("Nickels:",num_nickels)
print("Pennies:",num_pennies)
