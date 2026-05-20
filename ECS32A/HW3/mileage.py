# mileage.py
#
# calculate fuel economy

print("Your Personal Fuel Economy")

#sum of number of miles traveld seen so far
sum_miles_traveled = 0

#sum of gallons needed seen so far
sum_gallons_needed = 0

#loop obtaining number from user until they pressing enter key
while True:
    #get user input
    num_mile_traveled = input("Number of miles traveled (or enter to exit):")
    
    #determine if we should stop the loop
    if num_mile_traveled != "":
        #get user input for fuel needed
        num_gallons_needed = input("Number of gallons needed:")
        #convert for safety
        num_mile_traveled = float(num_mile_traveled)
        num_gallons_needed = float(num_gallons_needed)
        #calculate mileage
        mileage = num_mile_traveled / num_gallons_needed
        #formatted mileage
        formatted_mileage = "{:.1f}".format(mileage)
        print("Mileage this tank:",formatted_mileage)
        #cumulative miles travalled and gallons needed
        sum_miles_traveled = sum_miles_traveled + num_mile_traveled
        sum_gallons_needed = sum_gallons_needed + num_gallons_needed
        
    else:
        #calculate average mileage
        average_mileage = sum_miles_traveled / sum_gallons_needed
        #formatted average mileage
        formatted_average_mileage = "{:.1f}".format(average_mileage)
        print("Average mileage:",formatted_average_mileage)
        #end loop
        break    
