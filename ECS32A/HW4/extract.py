# extract.py
#
# extract 10 digit phone number whether the format that user entered.

phone = input("Enter phone number:")
extract_number = ""
#loop every number in phone number
for number in phone:
    #only store numbers 
    if number.isdigit():
        #stored number
        extract_number= extract_number + number
print(" Numbers:",extract_number)
