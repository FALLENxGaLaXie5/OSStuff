1. For the input, I tested and showed in the typescript, it's parsed and works perfectly; it grabs the customers and stores them in a dynamically allocated array.

2. The threads are created and work; however, I couldn't figure out how to pass the number of customers effectively, so I eneded up storing the number of customers in the first customer slot as the "cashier". So you can see that in me accessing it in the individual threads.

3. FCFS for the even lanes works as expected, but it's printing out some of the lanes multiple times for some reason. It does get the right values for the prices however.

4. Express was... interesting. I think I had issues storing the smallest customer at each point and then decreasing the size of the array. Anyways that doesn't really work, but I did write an algorithm for it.