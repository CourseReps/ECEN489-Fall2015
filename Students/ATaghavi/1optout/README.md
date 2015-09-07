#Opt-Out 1: Hamming Codes
This folder contains my source and CMakeLists files for opt-out1, writing a hamming encoder, decoder, and error checker in C++ (for the 7,4 Hamming code algorithm). If the input has a length that is not divisible by 4, then it will pad the string with 0's to achieve a correct length for encoding with the 7,4 encoding. If a string is to be checked or decoded and does not have a length divisble by 7, then the excess bits will be ignored. 

###Usage
There are 3 usages of the program. First, it can be used to encode an abritrary length binary string with the 7,4 Hamming encoding. To use the program in this way, run the program with the arguments as follows: encode [binary string]. Example: ./HammingCodes encode 1011

The second usage is to decode a Hamming code. This will simply decode the string, and not check for errors. Usage is as follows: decode [binary string]

Lastly, the program can be used to check a Hamming encoding for 1-bit errors. Its usage is as follows: check [binary string]. The program will output the indices of the error(s) and the corrected code.
