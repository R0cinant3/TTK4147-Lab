# Create an array with some small capacity, then insertBack more elements than there is capacity for, and run the program. What happens?
By inserting more elements than there is capacity for, the program crashes, where a segmentation fault has happened. This means that the program tried to access a restricted portion of memory when the memory space available to the program is full.

# Compile and run the program again, but now add the compiler flags -g -fsanitize=address. What happens now?
The program does not get compiled because the AddressSanitizer tool detects memory errors in the code.