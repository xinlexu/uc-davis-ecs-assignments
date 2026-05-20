# phone.py
#
# Validate a phone is specified in the form (###)###-####

phone = input("Enter number as  ###-###-####:")
#check length
valid = len(phone) == 12
pos = 0
#loop to check the phone number
while valid and pos < 12:
    #check "-"
    if pos == 3:
        valid = phone[pos] == "-"
    elif pos == 7:
        valid = phone[pos] == "-"
    #check digit
    else:
        valid = phone[pos].isdigit()
    pos = pos + 1

if valid:
    print("Valid")
else:
    print("Invalid")
