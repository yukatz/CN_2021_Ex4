#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <time.h>
#include <signal.h>
#include <arpa/inet.h>

int main()
{
    int sock = 0;
    struct sockaddr_in measureAddress;
    
    for(int i = 1; i <= 5; i ++){
	   char buffer[65536]={0};
	   if((sock = socket(AF_INET, SOCK_STREAM,0))<0)
	   {
		 printf("Could not create socket : %d" ,errno);
		 return -1; 
	   }
		
       memset(&measureAddress, 0, sizeof(measureAddress));
       measureAddress.sin_family = AF_INET;
       measureAddress.sin_port = htons(5060);
	
	   if (inet_pton(AF_INET, (const char*)"127.0.0.1", &measureAddress.sin_addr) <= 0){
		  printf("inet_pton() failed");
		  return -1;}
printf("connecting...\n");
       if (connect(sock, (struct sockaddr *) &measureAddress, sizeof(measureAddress)) == -1)
	   {
	       printf("connect() failed with error code : %d" ,errno);
		   return -1;
	   }

       printf("Connected to server, sending file\n");
	   
       FILE *MyF; 
	   MyF = fopen("MyF.txt", "r");
       if(MyF == NULL)
	   {
        	perror("File doesn't exists");
        	return -1;
       }
		
       printf("File %d\n", i);
	   
	   int messageLen;
	 
	   while((messageLen = fread(buffer, 1, sizeof(buffer), MyF))>0){ 
	      int bytesSent = send(sock, buffer, messageLen, 0);
          if (-1 == bytesSent)
        {
	      printf("send() failed with error code : %d",errno);
        }
        else if (0 == bytesSent)
        {
	    printf("peer has closed the TCP connection prior to send().\n");
        }
        else if (messageLen > bytesSent)
        {
	    printf("sent only %d bytes from the required %d.\n", messageLen, bytesSent);
        }
        else 
        {
	    printf("File sent .\n");
        }
	 
    fclose(MyF);
    close(sock);
   }
   }
   ////////Reno////////////
     printf("Changing algorithm!\n");
	 for(int i = 0; i <= 5; i ++){
	 	   char buffer[65536]={0};
		sock = 0;
		
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    	{ 
        	printf("\n Socket creation error \n"); 
        	return -1; 
    	} 
		
     	memset(&measureAddress,0, sizeof(measureAddress));  
    	measureAddress.sin_family = AF_INET; 
    	measureAddress.sin_port = htons(5060); 
        if(inet_pton(AF_INET, (const char*)"127.0.0.1", &measureAddress.sin_addr)<=0)  
    	{ 
        	printf("\ninet_pton() failed \n"); 
        	return -1; 
    	}
		
		
		strcpy(buffer, "Reno"); 
		if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, strlen(buffer)) != 0) {
			perror("setsockopt"); 
			return -1;
		}
		socklen_t len = sizeof(buffer);
		if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &(len)) != 0) {
			perror("getsockopt"); 
			return -1; 
		} 
        printf("Algorithm changed to %s \n" , buffer);
    	
		printf("connecting...\n");
    	if (connect(sock, (struct sockaddr *)&measureAddress, sizeof(measureAddress)) < 0) 
    	{ 
        	printf("\nConnection Failed \n"); 
        	return -1; 
    	} 
    		
		FILE *MyF; 
		printf("Sending file... %d\n",i);
		MyF = fopen("2mb.txt", "rb");
        if(MyF == NULL){
        	perror("Can't read file.");
        	return -1;
        }
        int messageLen = 0;
        int bytesSent = 0;
        int count = 0;
        int amountSeg = 0;
        bzero(buffer, 65536);
    	while((messageLen = fread(buffer, 1, sizeof(buffer), MyF))>0 ){
			 bytesSent = send(sock, buffer, messageLen, 0);
		     if (-1 == bytesSent)
		     {
				printf("Can't send file: %d", errno);
		     }
		     else if (0 == bytesSent)
		     {
				printf("peer has closed the TCP connection prior to send().\n");
		     }
		     else if (messageLen > bytesSent)
		     {
				printf("sent only %d bytes from the required %d.\n", bytesSent, messageLen);
		     }
		     else 
		     {
				printf("File sent .\n");
		     }
		    
   		}
		fclose(MyF);
		close(sock);
	}

	return 0; 
} 

