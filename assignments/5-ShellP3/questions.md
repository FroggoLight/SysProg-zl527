Please read this before grading, I beg you plssssss

I just discovered that my environment is slightly different from the others where my dsh works but others will immediately run into seg fault, this is because of a single line which I thought should not be changed due to how it was implemented as the template for the past few assignments:

In dshlib.c, line 57:

- char *cmd_buff;

Instead, make a copy and replace this line with:

- char cmd_buff[ARG_MAX];

so that it works with the general environment. I have no idea why my macbook works but any other environment hates it. Please have mercy on my grades since the last two assignments I wrote this and other TAs gave me a seg fault, and this one is worth way more for the correct implementation


1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

This implementation allows all child processes to finish running before we get to the next user input by using waitpid for all child processes, as it suspends execution of the parent process until everything from the child is finished. Without waitpid on the child processes, we can potentially create orphaned child processes while the parent returns and our shell waits for our next user input. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

One of the ways it can go wrong if we leave the pipes open we can get unintended data flow or hanging processes awaiting for the other process to write more to the pipe.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

If we were to implement cd as an external process, this would only result in the working directory being changed for the temporary child process and not the parent process. This does not translate to what we have in our own shell and thus we have to use chdir() for the navigation. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

The modification would be first be placed on the command list struct, where instead of having 8 max commands we would make it a list of commands where then we have to change the implementation to dynamically allocate each commands within the build command list and build command buff. Realloc should also be used to make repeated allocation more efficiently especially where we would have a dynamically sized array of commands. However, the trade off could be the runtime where the dynamically allocated array would need to resize everytime until it fits the required amount of pipes and for shrinking as well if there are not much piping happening. 
