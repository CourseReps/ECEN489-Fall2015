#Hamming(7,4)

Using CMake, there will be three executables generated:

### BinaryGenerator 
Generates a random binary string, with an option for how many 4-bit words to generate. 
Takes an output file as an argument. Example: ./BinaryGenerator binary.txt

### Encode 
Encodes a given binary file using the Hamming(7,4) code. Takes an input file and output file 
as arguments. Example: ./Encode binary.txt encoded.txt

### Decode 
Decodes a given binary file encoded with the Hamming(7,4) code. Takes an input file and output 
file as arguments. Example ./Decode encoded.txt decoded.txt

A general way to test the functionality is to use BinaryGenerator to generate a random string of binary digits. Then, pass this file to Encode to get the encoded string. You can then change up to 1 bit per 7-bit codeword to introduce "noise". Finally, you can use Decoder to decode the string, and run a diff between the decoded file and the original binary file.
