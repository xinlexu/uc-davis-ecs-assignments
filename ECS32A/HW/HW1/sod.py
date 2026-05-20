#sod.py

length = input("Enter length:")
length =int(length)
width = input("Enter width:")
width =int(width)
piece = (length * width) / 9 + 1
piece = int(piece)
print("You should order", piece, "pieces.")
