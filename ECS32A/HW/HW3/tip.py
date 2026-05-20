# tip.py
#
# tip from 15% to 25% for the total bill

#ask user for total bill
total_bill = float(input("Enter total bill:"))

#loop prints each tips from 15% to 25%
pct = 15
while pct <= 25:
    #calculate the tip
    tip = (pct/100) * total_bill
    
    #format the pct and tip string
    formatted_str_pct = "{:.0f}%".format(pct)
    formatted_str_tip = "${:.2f}".format(tip)

    #print the result
    print(formatted_str_pct,"is",formatted_str_tip)
    
    pct = pct + 1
