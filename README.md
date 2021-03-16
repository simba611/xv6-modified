To run this use - 
    make clean; make qemu-nox SCHEDULER=<RR, FCFS, PBS, MLFQ>

# TASK 1 

## waitx

- ### waitx system call
    The waitx call has the same function as wait i.e. it searches for a ZOMBIE child of a parent. If it finds a child, *wtime and *rtime are updated. 
    Proc structure was changed to keep track of ctime, rtime and etime. Another variable for iotime was also added.
    ctime is calculated when a process is at EMBRYO stage.
    etime is calculated when a process exits.
    rtime is calculated by incresing the running time of the process with RUNNING status every tick.
    iotime is calculated by getting the difference between wakeup and sleep calls.
    We get *rtime from proc structure and *wtime as etime-ctime-rtime-iotime. 

- ### time user program
    It inputs a command and executes it with waitx instead of wait. This will display the rtime and wtime by using waitx sustem call

## ps

- ### ps system call
    A ps system call was created to implement the ps user program. 
    We iterate through the ptable and for every proc structure which is not UNUSED we use the proc structure to get all the data.
    w_time is ticks - p->queue_time where p->queue_time is the time it was last inserted into the queue.
    w_time is set to 0 for RUNNING process.
    Current queue is defined as 0 in case of non-MLFQ scheduling.
    NRUN i.e. p->cpu_call is updated in all schedulers as required and q[i] are updated in MLFQ.
    

- ### ps user program
    The ps user program is used to call the ps system call.

## set_priority

- ### set_priority system call-
    The set_priority system call changes the priority of a pid by altering p->priority inside ptable.lock. It also returns the old priority of this process.

- ### setPriority user function-
    The setPriority user function takes pid and newpriority as input and passes it to the set_priority system call. 


# TASK2 - 
To implement SCHEDULER flag, the -D flag of gcc was used.
- ## FCFS scheduling - 
    We iterate through the process table and find the RUNNABLE process with minimum creating time. We only execute the last running process with minimum creation time till it goes to sleep or exits, after that we look for the process with minimum creation time. NRUN is increased only when is process is scheduled for the first time or after it comes back from sleep.

- ## PBS scheduling - 
    We assign a default value of 60 to every process' priority. We iterate through the ptable and find the process with minimum priority number.
    To implement round robin for same priority number processes , we choose the first process with minimum priority number. A new search happens every tick making sure that after every time slice the process with minimum priority number is selected.

- ## MLFQ scheduling - 
    For MLFQ scheduling, we add every new process to the end of the highest priority queue. To choose a process we iterate through the ptable and find the RUNNABLE process with minimum queue number which was added to that queue earliest. Aging is implemented by checking the age of all the processes after every tick. If it exceeds a certain time we add it to the back of the next higher priority queue(unless already in queue 0). This time is chosen to be different for different queues. For any queue i, it is chosen to be 5*(2^i). If a process in queue i is run for 2^i time slices it is moved to the end of next queue. Number of CPU calls is only increased if a new timeslice begins for the process. A process is added to the back of its queue if it comes back after I/O. q[i] is the number of ticks the process has received so this value is increased at the beginning of the tick. For the last queue a round robin scheduler is implemented by putting a process to the end of the queue after every 16 ticks and looking for the next process. A process can take advantage of the scheduling policiy by giving up control of the CPU before it's time slice is over for a very small time and then joining back the same queue. This was the process will maintain higher priority and have a lot of CPU time too.

## Comparison between MLFQ and the other sceduling algorithms
For comparing we used the benchmark program provided with waitx system call to measure the waiting time of all the child processes. The benchmark program calls different types of processes ranging from completely I/O bound to completely CPU bound. The following total waitimg times(ticks) were observed for different processes - 
- ### FCFS
    14786, 15492, 14669
- ### RR
    10660, 10947, 11543
- ### MLFQ
    11357, 11882, 12258
- ### PBS 
    - #### With same priority for all
        11453, 10926
    - #### With priority assignment when I/O bound processes are given higher priorirty
        6439, 7087
    - #### With priority assignment when CPU bound processes are given higher priority
        12904, 12634 





# Bonus

## I/O intensive processes
Below is the graph when all the processes were I/O intensive.
As all were I/O intensive they do not exceed their time slice executing commands and were not demoted to lower queues.

![Alt text](graphs/all_io.png?raw=true "Title")

## CPU intensive processes
Below is the graph when all processes were CPU intensive. 
As all were CPU intensive, most of the time was spent executing instructions and they exceeded the time slice limit to be demoted to lower queues. 

![Alt text](graphs/all_cpu.png?raw=true "Title")

## Half I/O based, half CPU based
Below is the graph where 2 of the processes were I/O intensive and 2 were CPU intensive.
Here the 2 processes reaching queue 4 were CPU intensive and exceeded their time slices. The rest of the 2 were I/O intensive and didn't exceed their time slice executing instructions.

![Alt text](graphs/2_io_2_cpu.png?raw=true "Title")

## All different
The 2 graphs below show the plot where all four processes had different I/O and CPU load.
Here all processes have different I/O and CPU loads. We gradually switch between I/O bound process and CPU bound process.
This can be observed by the observing the transition of the plots from CPU intensive to I/O intensive.  
The CPU bound processes occupy the CPU for longer periods of time and get demoted to lower priority queues while the I/O bound processes do not exceed their time slices while executing and do not get demoted to lower priority queues.
![Alt text](graphs/all_diff.png?raw=true "Title")

![Alt text](graphs/all_diff2.png?raw=true "Title")
