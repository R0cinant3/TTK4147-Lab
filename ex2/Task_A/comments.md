

# What Happens when running the program?
When the program is running, the 8GB are allocated. However, because this is the limit, the program will crash, and the swap memory is then full. The RAM allocation is then returned. 

# What is the difference between "Memory" and "Swap"?
Memory is RAM memory while Swap is located on the hard drive. Thus, memory is faster than Swap. 
Swap space is memory used when the RAM is full. If the system needs more memory resources and the RAM is full, inactive pages in memory are moved to the swap space.

# Now change the shape of the matrix by increasing x dim first to 1000, then 10000, and run the program again. What happens?
When the x dimension is increased to 1000, the amount of RAM allocated is reduced by 3.9GB, meaning that the program size is halved. It is now able to run the program without returning "Killed", where the RAM is deallocated and swap is filled.
When the x dimension is increased to 10000, the amount of RAM allocated is reduced by 7.3GB compared to the original x size of 100. Thus, the program has gone from using 8GB of RAM to 700MB.  

# Run the program with x dim = 10000 again, but this time write zeros to all the memory. Explain why something different happens when the memory is also cleared.
The program goes back to using 8GB. The reason is that all the memory locations are set to 0 of type long. This means that there are now 8 bytes per