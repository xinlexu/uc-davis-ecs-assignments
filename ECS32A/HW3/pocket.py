6#pocket.py
#
#Ask the user number of each kind of money and calculate the total numbers of dollars and cents.

print("Pocket change calculator")

#ask user about number of each kink of money
num_quarters = input("How many quarters?")
num_dimes = input("How many dimes?")
num_nickels = input("How many nickels?")
num_pennies = input("How many pennies?")

#convert value of money into dollor
value_quarters = float(num_quarters) * 0.25
value_dimes = float(num_dimes) * 0.1
value_nickels = float(num_nickels) * 0.05
value_pennies = float(num_pennies) * 0.01

#calculate total amount in dollor
amount = float(value_quarters) + float(value_dimes) + float(value_nickels) + float(value_pennies)

#output
formatted_str = "${:.2f}".format(amount)
print("You have",formatted_str)
