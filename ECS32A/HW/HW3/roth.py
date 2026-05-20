# roth.py
#
# calculates the number of months it takes for an investment made in a Roth IRA to double in value.

# prompt the user for an initial Roth IRA deposit amount.
Roth_IRA = float(input("Enter an initial Roth IRA deposit amount:"))

# prompt the user for an initial investment amount APR.
APR = float(input("Enter an annual percent rate of return:"))
monthly_rate = (APR/100)/12

#current month
month = 0

#current value
current_value = Roth_IRA

#loop until investment doubled
while current_value < Roth_IRA * 2:

    #calculate interest
    interest = current_value * monthly_rate

    #add interest to Roth IRA as current value
    current_value = current_value + interest

    #advance the month
    month = month + 1

    #value formatted in dollors
    formatted_current_value = "${:.2f}".format(current_value)
    print("Value after month",str(month)+":",formatted_current_value)
    
#APR formatted in percentage
formatted_APR = "{:.0f}%".format(APR)

#print out when roth IRA doubled
print("It will take",str(month),"months to double your investment with a",formatted_APR,"return.")

