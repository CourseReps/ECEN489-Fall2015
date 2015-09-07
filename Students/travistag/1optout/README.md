#Hamming(7,4)

Using CMake, there will be three executables generated.

BinaryGenerator - generates a random binary string, with an option for how many 4-bit words to generate. 
Takes an output file as an argument. Example: ./BinaryGenerator binary.txt

Encode - encodes a given binary file using the Hamming(7,4) code. Takes an input file and output file 
as arguments. Example: ./Encode binary.txt encoded.txt

Decode - decodes a given binary file encoded with the Hamming(7,4) code. Takes an input file and output 
file as arguments. Example ./Decode encoded.txt decoded.txt
