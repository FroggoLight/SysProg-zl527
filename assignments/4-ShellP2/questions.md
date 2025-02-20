1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Calling just execvp would take over our process and thus not be able to execute the next few lines that we have written, thus we need to use fork to create a child process that will run the requested process and allow our current parent process to proceed normally

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  It means that it did not create a new process. For me, I just exited the program. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp looks for the PATH environment variable to know and find the command to execute. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  It is to prevent child processes in becoming a zombie process where we will not be able to access the resources that the zombie process have. If we don't call wait, the child process will remain in the system consuming resources that will not be accessible. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It extracts the exit status of a child process. This information allows us to understand if the child process ran successfully and is useful for narrowing down debug issues, and it allows parent process to control in case of a success or failure of the child process. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  For my implementation, my while loop checks to see if there is a quote character, and in that case I will iterate through the whole string until another quote character is met and then copy the string into the argv. This is necessary as without the implementation the program will treat each space as a word regardless of the quote and could run into errors specifically with echo.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  There was a lot more challenges, especially where I had to drop using strtok as it does not work with quotation. At the end there was not much of a refactoring but a lot of rewriting.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  It is used to communicate process events between the kernel or processes. Rather than using it for communication like IPC, it is more suited for process controls.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL guarantees the termination of a process, SIGTERM requests the termination of a process but can be catched for cleanup, and SIGINT is where the user performs the interruption of a process. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  It pauses a process until it receives a signal to continue execution. It can not be caught or ignored as it is designed with the intent for debugging or process control. 
