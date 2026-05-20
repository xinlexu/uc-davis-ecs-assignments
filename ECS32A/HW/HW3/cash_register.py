#cash_register.py
#
# ask the price from user and print number pf items and total price

print("Cash register")

#track number of items and price
num_item = 0
total_price = 0

while True:
    price_item = input("Enter the price of an item:")
    if price_item == "":
        break
    else:
        total_price = float(price_item) + float(total_price)
        num_item = num_item + 1
    
    
print("You entered",num_item, "items totaling","${:.2f}".format(total_price))
