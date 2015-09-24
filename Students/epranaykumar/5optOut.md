Lets say there are 4 clients and one server. Clients will be using UDP to broadcast   
  
1. What is the content and format of the client's broadcast message? (if JSON, then the attribute name and value have to be standardized)  
  
      -  The content should help the server to identify the client   
      -  For the server to reply back, the content should contain current IP of the client    
  

   
2. When broadcasting using UDP, the order of the messages is not guaranteed.  
  
      - whatever the message the client sends,  it should be in one packet (only one send function).  
  
3. How to determine the size of the receiving message at the server?  
      - one way is to use string message and find the end of character   
  
4. As it is UDP, the server has to reply back to make sure the client message has received the server. So,the format of the reply message ?     
   
5. As there are multiple clients, we have to implement asynchronous UDP server ?

6. What has to be done to ensure secure communication?    

     - The message has to be encrypted     
     - Client has to authenticate itself to server before sending the relevant information 
