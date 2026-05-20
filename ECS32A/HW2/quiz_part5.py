#quiz_part5.py
#
#meaning of score

x = 0 #reset x as score
#question1
print("ART AND LITERATURE: Who painted Starry Night?")
print("a. Vincent van Gogh")#Correct
print("b. Michelangelo")
print("c. Leonardo da Vinci")
#input answer
answer = input("Enter your choice:")
#check answer
if answer == "a":
    print("Correct!")
    x = int(x+1) #gain 1 point
else:
    print("The correct answer was a")
    
#question2
print("ENTERTAINMENT: How many oscars did Alfred Hitchcock win?")
print("a. None")#Correct
print("b. One")
print("c. Two")
#input answer
answer = input("Enter your choice:")
#check answer
if answer == "a":
    print("Correct!")
    x = int(x+1) #gain 1 point
else:
    print("The correct answer was a")

#question3
print("GEOGRAPHY: Which is the largest ocean?")
print("a. Pacific")#Correct
print("b. Atlantic")
print("c. Indian")
#input answer
answer = input("Enter your choice:")
#check answer
if answer == "a":
    print("Correct!")
    x = int(x+1) #gain 1 point
else:
    print("The correct answer was a")

#question4
print("HISTORY: Who was the first U.S. president to appear on a coin?")
print("a. Washington")
print("b. Lincoln") #Correct
print("c. Jefferson")
#input answer
answer = input("Enter your choice:")
#check answer
if answer == "b":
    print("Correct!")
    x = int(x+1) #gain 1 point
else:
    print("The correct answer was b")

#question5
print("SCIENCE AND NATURE: Can pigs swim?")
print("a. Yes") #Correct
print("b. No")
print("c. Only in salt water")
#input answer
answer = input("Enter your choice:")
#check answer
if answer == "a":
    print("Correct!")
    x = int(x+1) #gain 1 point
else:
    print("The correct answer was a")

#question6
print("SPORT AND LEISURE: What color is the middle Olympic ring?")
print("a. Red")
print("b. Blue")
print("c. Black") #Correct
#input answer
answer = input("Enter your choice:")
#check answer
if answer == "c":
    print("Correct!")
    x = int(x+1) #gain 1 point
else:
    print("The correct answer was c")

#Genius question
print("GENIUS: What is D divided by X?")
#input answer
answer = input("Enter your answer:")
#check answer
if answer == "L" or answer == "50":
    print("Correct!")
    x = int(x+1)
else:
    print("Correct answers were L or 50")

#final score
print("Your final score is "+str(x))

#score meaning
x = int(x)
if x >= 0 and x <= 2:
    print("You were unlucky!")
elif x >= 3 and x <= 4:
    print("You did better than chance!")
elif x >= 5 and x <= 6:
    print("You are a trivia star!")
else:
    print("Genius!")
