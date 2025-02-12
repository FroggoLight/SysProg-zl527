1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  This allows a line by line reading, essentially replicating the way the shell works

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  for this case, we do not necessarily know the size of which the user inputs have and therefore its not recommended if the user puts in something that is greater than the intended stream size


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Without trimming, the parser for each commands may break and spit out unwanted actions 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  We should implement <, >, and >>. There should not be as much of a challenge for > and < as we can use read and write most likely while >> could be a bit more difficult but should be doable if we know the ending of the target file.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  redirection is more of just directing file streams from either input to target or output to target, while piping deals with output stream and is chained consecutively. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is to let the programmer or the person know if the shell encountered an issue or not. Without the distinction we might not know that the process was ever completed fully.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Stderr should be priortized in all of the chain of commands in order to let the person know that something went wrong even if all other commands have been executed successfully. We could merge the outputs if we can find a way to distinct between where there is an error or not.