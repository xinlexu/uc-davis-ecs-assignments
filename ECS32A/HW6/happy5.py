# happy5.py
# Looking up the happiness index in the dictionary
# If the country does not exist in the dictionary, no output should be printed for that country.



def main():
    
    # Part 1
    # Build dictionary mapping countries to happiness index
    happy_dict = make_happy_dict()

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
    read_gdp_data(happy_dict)


def make_happy_dict():
    happy_dict = {}
    #open happiness.csv
    infile = open("happiness.csv")
    #ignore the header
    infile.readline()
    for line in infile:
        line = line.strip()
        country,year,happiness_index = line.split(",")
        happy_dict[country] = happiness_index
    return happy_dict

def read_gdp_data(happy_dict):
    #open world_pop_gdp.tsv
    infile_2 = open("world_pop_gdp.tsv")
    first_line = infile_2.readline()
    first_line = first_line.strip()
    first_line = first_line.replace("\t",",")
    first_line = first_line.replace("\n",",")
    print(first_line+","+"Happiness")
    
    #loop each line in tsv file 
    for lines in infile_2:
        #remove space
        lines = lines.strip()
        cols = lines.split("\t")
        #assign value in the list
        country,population,GDP = cols[0],cols[1],cols[2]
        #remove commas
        GDP = GDP.replace(",","")
        population = population.replace(",","")
        #remove dollar sign
        GDP = GDP.strip('$')
        #find country in the dictionary
        if country in happy_dict:
            #value return to the key in dictionary
            happiness_index = happy_dict[country]
            #remove newline 
            happiness_index = happiness_index.replace("\n","")
            print(country+","+population+","+GDP+","+happiness_index)
        
        
    return

def lookup_happiness_by_country(happy_dict):
    while True:
        country_name = input("Enter a country to lookup or 'done' to exit:")
        if country_name == 'done':
            return
        if country_name in happy_dict:
            print(happy_dict[country_name])
        else:
            print(country_name, 'not found')

# Function prints all the values in a dictionary d sorted by key
def print_sorted_dictionary(D):
    if type(D) != type({}):
        print("Dictionary not found")
        return
    print("Contents of dictionary sorted by key.")
    print("Key","Value")
    for key in sorted(D.keys()):
        print(key, D[key])
        
main()

