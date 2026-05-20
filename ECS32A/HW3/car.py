# car.py
#
# guess price game to win the car


print("Guess the price and win the prize!")
#track number of guessing
guess_number = 0

#loop until the user got right answer
while True :
    guess = int(input("Enter your guess:"))
    #check the answer
    if guess > 42500:
        print("Too high!")
        guess_number = guess_number + 1
    elif guess < 42500:
        print("Too low!")
        guess_number = guess_number + 1
    else:
        guess_number = guess_number + 1
        if guess_number <= 5:
            print("You won the car!")
            break
        else:
            print("Too many guesses!")
            break
            
