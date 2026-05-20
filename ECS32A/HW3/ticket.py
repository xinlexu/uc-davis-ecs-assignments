# ticket.py
#
# compute the price by asking the age

#asking user age
age = int(input("Enter age:"))

#check age range
if age < 3:
    print("Price: FREE")
elif age >=3 and age <= 12:
    print("Price: $29.95")
elif (age > 12 and age < 18) or age >= 65:
    print("Price: $39.95")

#ask for college id
else:
    ans_ID = input("College ID? (y/n)")
    if ans_ID == "y":
        print("Price: $39.95")
    else:
        print("Price: $49.95")
