1. Commented out a lot of the structured code that actually controls the monitor. I initialized the
states in main, but it was giving me "variability errors" whenever I tried to use it, and I wanted to make sure
everything compiled, so I ended up just commenting most of it out.

I feel like I understood what the goal of the monitor was; the state of each philosopher is stored
    in the states array, and when the philosopher goes to pick up the chopsticks, the monitor checks to see
    which other philosophers are eating and what states they're in, so that it can check to see if the chopsticks
    needing to be picked up are in use.

I also did have a bit of trouble understanding elements of the pseudo-code given. What was the self condition array for?

Anyways, it was a very interesting problem, and I wish I could've gotten task 3 to work completely. Task 1 and 2 were pretty
interesting experiments and good segways into task 3.