# happy1.py
#
# Build dictionary mapping countries to happiness index

def main():

    # Part 1
    # Build dictionary mapping countries to happiness index
    happy_dict = make_happy_dict()
    print_sorted_dictionary(happy_dict)

    # Part 2
    # Print key value pairs sorted by key
    # Uncomment the function call below for part 2 only
    # print_sorted_dictionary(happy_dict)

    # Part 3
    # Uncomment the function call below for part 3 only
    # lookup happiness by country until the user enters done
    # lookup_happiness_by_country(happy_dict)

    # Parts 4-6
    # Uncomment the function call below for parts 4-6 
    # Read file containing population and GDP data and add happiness data

def make_happy_dict():
    happy_dict = {}
    #open happiness.csv
    infile = open("happiness.csv",'r')
    #ignore the header
    infile.readline()
    for line in infile.readlines():
        line = line.strip()
        country,year,happiness_index = line.split(",")
        happy_dict[country] = happiness_index
    infile.close()
    return happy_dict

def read_gdp_data(happy_dict):
    return

def lookup_happiness_by_country(happy_dict):
    return

# Function prints all the values in a dictionary d sorted by key
def print_sorted_dictionary(D):
    for key in sorted(D.keys()):
        print(key, D[key])
        
main()
