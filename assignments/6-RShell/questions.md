Please read this: I have yet again found a new problem that my environment somehow does not mind but does it differently than the rest. I have implemented the fix which should allow multiple runs in a single session. I swear if my macbook messes me up for this class I will die internally and cry for a week (my previous assignments got nuked because of this macbook "not" caring about uninitialized stuff and how memory allocation works)

1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

We used the send_message_eof where we specified that when the client reads the specified EOF character it will know that the output has been fully received. We can also implement timeout where we give the server a set time for the response

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

In our implementation, we utilized the EOF character to determine when to stop the client from continuously receiving the stream. If this is not handled correctly, we can get the client side to hang and not get any feedback. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocol maintains the data or info on the past interactions, meaning that in the server client context it uses the past information to process requests. Stateless protocol does not maintain the info on past interactions, meaning that the communication is much more lightweight without depending on previous interactions but can become more of a problem if the communication design wants to be more stateful where each communication adds more info to process requests.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP has its own purpose for being lightweight and generally used for anything that requires fast transmissions like video streams where losses can be acceptable. Other protocols have their own purpose for maintaining reliability that needs to have a guarantee for the transmission. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The os provides a socket api which allows applications to use network communication, and supports various protocols for general use. 

Please have mercy on my grades