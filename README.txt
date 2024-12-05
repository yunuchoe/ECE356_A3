ECE 356 - Assignment 3 README.txt FILE

Question 1:

    step1_2: Since step 1 is very simple and is just setting up the threads and reources, I combined
    it with step 2. Step 2 involves the program which can have potential deadlocks. I tied this randomness 
    meaning that a resource could possibly be hogged when a thread does not have eveyrthing to complete. However,
    most of the time a dead lock was not created. The outputted log would simply be a program that isnt finished
    but also wont end either so I did not include it.

    step3: this step was simple and just involed ensuring that control was given before the runway semaphore.
    This ensures there will never be a deadlock due to resource constraints

    step4: this was more involved and required each thread to count how long they were waiting. If something
    waited a long time, I moved it the front of the queue. However, as moving the queus take a some time, and
    each thread is already guarenteed (no deadlock) to eventaully finish, I am not sure if this makes the system for efficent
    However, it does guarente some amount of fairness. Depsite that, the queue was always quite fair as it is based off order
    of creation. This system just adds an extra layer of security to it.
    I also included a log at the end out the code that outputs to the terminal. This showcases which plane waited the longest


Question 2:

    The method I used to make this multithreaded server.c is heavily based off of the code from the lecture slides.
    However, I made some decent adjustments to better simulate the outlinted constraints. I have also put the order I
    tested to get the deisred output. However, due to the nature of multi threaded processes, I noticed that sometimes
    the client 1 sends a message and then leaves right away. This is not what is desired but is a product of the nature
    of multi threads. Since they all run at the same time, its possible for client1 to leave before client 2 says anything.
    I would say its about a 50/50 odds. However, if more clients were added, the large multithreaded process might better
    replicate what we want. However, my solutions only contains the 2 client version.

    client1.c and client2.c are also pretty simialr to the lecture notes. The only major change is the message.


    Order:
        server.c: 
        gcc -o server.o server.c 
        ./server.o

        client1.c:
        gcc -o client1.o client1.c
        ./client1.o

        client2.c:
        gcc -o client2.o client2.c
        ./client2.o

    Best case output:
    ./server.o 

    Chat sever started. Waiting for clients...
    New client conneceted: Client 1
    New client conneceted: Client 2
    Client 1: Hi Client 2!
    Client 2: Hello, everyone!
    Client 2 disconnected.
    Client 2 disconnected.
