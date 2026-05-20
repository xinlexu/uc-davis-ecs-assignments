#heart.py

age = input("Enter your age:")
age = int(age)         
maximum = 220 - age
target_heart_rate_low = maximum * 0.5
target_heart_rate_high = maximum * 0.85
print("Your maximum heart rate is", maximum, "bpm")
print("Your target heart rate is", target_heart_rate_low, "-", target_heart_rate_high, "bpm")
