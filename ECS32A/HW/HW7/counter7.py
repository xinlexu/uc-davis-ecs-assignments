# counter6.py
#
# Find and return the most frequent words
#
# import wordle
import string

#
# The Count class
#
# The class keyword below begins the definition of a new Python data type Count.
# Count keeps word counts. All the methods that we can use with Count objects
# and the variables (attributes) built inside Count objects are defined in the
# this class definition.
#
class Count:
        #
        # Count constructor
        #
        # the __init__ method initializes the variables (attributes)
        # in Count objects, such as the dictionary datastructure that
        # hold the counts for each word. It is called when the count
        # object is first created using Count().
        #
        # The variable self refers to the object being initialized
        # So self.word_counts is referring to the word_count dictionay
        # within the object.
        #
        # Always add self in front of word_counts to access it within a method.
        def __init__(self):
                print("Word Counter Initialized")
                # Initialize word_counts to an empty dictionary
                self.word_counts = {}
                self.stop_words = []
                try:
                    infiles = open('stop_words.txt')
                    for line in infiles:
                        for word in line.strip().split():
                                word = word.lower().strip(string.punctuation)
                        if len(word) > 0:
                                self.stop_words.append(word)
                    infiles.close()
                except:
                    pass

        # The get_num_words method returns the number of words
        # (keys) in the word_counts dictionary.
        # This counts each word only once.
        def get_num_words(self):
                return len(self.word_counts)

        # The increment_count method increments the count of a word. 
        # If word is not yet in the dictionary, self.word_counts,
        # it is added with a count of 1. If word is in the dictionary,
        # its count is incremented by 1.
        def increment_count(self,word):
                word = word.lower()
                word = word.strip(string.punctuation)
                if len(word) > 0 and word not in self.stop_words:
                        if word not in self.word_counts:
                                self.word_counts[word] = 0
                        self.word_counts[word] = self.word_counts[word] + 1

        # The lookup_count method returns the count of word
        # from the self.word_counts dictionary. If the word
        # is not in the dictionary, it returns 0.
        def lookup_count(self,word):
                if word not in self.word_counts:
                        return 0
                return self.word_counts[word]

        # The get_top_words method gets the words with the highest
        # counts out of the dictionary.
        #
        # The parameter num indicates how many words to return.
        #
        # The method returns a list of num (count,word) tuples
        # sorted from higest to lowest.
        def get_top_words(self,num):
                top = []
                for word, count in self.word_counts.items():
                        top.append((count, word))
                top.sort(reverse=True)
                return top[:num]
        
# The main program 
def main():
                                
        ## Make a new Count object 
        ## the counter object will keep track of
        ## the counts for all the words in the book
        counter = Count()

        ## For Part 5 Onward
        ## Open the user specified book file
        
        filename = input("Enter book file:")
        try:
                infile = open(filename)
        
        ## Put a loop here that extracts all words and
        ## inserts each word into the counter object by calling
        ## the counter.increment_count() method
                for line in infile:
                        for word in line.strip().split():
                                counter.increment_count(word)
        except FileNotFoundError:
                print(filename + " does not exists!")
        ## Test code for Part 7. 
        ## Uncomment for Part 7.
        print("Top 10 Words:")
        top_ten = counter.get_top_words(10)
        print(top_ten)
        print("Unique words:", counter.get_num_words())
        
        ## Finally, after you have submitted all parts
        ## of the assignment, uncomment the call to the
        ## wordle method below!
        ##
        ## You do not have to submit this part. 
        ##
        ## If there is a problem with your Tk installation
        ## the lab computers have been set up to run the code.
        ##
##        wordle.wordleFromObject(counter,30)

# Call the main program
main()
        
