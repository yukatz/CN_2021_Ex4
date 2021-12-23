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


#define SERVER_PORT 5060  //The port that the server listens
  
int main()
{
    int measureSocket = -1;  //Open listening socket for the mesure (as server)
    if((measureSocket = socket(AF_INET , SOCK_STREAM , 0 )) == -1)//creating check
    {printf("Could not create listening socket : %d", errno);}

	
    int enableReuse = 1;
    char buffer[65536]={0};
    if (setsockopt(measureSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enableReuse, sizeof(enableReuse)) < 0)
    {printf("setsockopt() failed with error code : %d",errno);}

    struct sockaddr_in measureAddress;
	printf("Socket created\n");
	
	memset(&measureAddress, 0, sizeof(measureAddress));
    measureAddress.sin_family = AF_INET;
    measureAddress.sin_addr.s_addr = INADDR_ANY;
    measureAddress.sin_port = htons(SERVER_PORT);
	
    printf("Bind\n");
	
    if (bind(measureSocket, (struct sockaddr *)&measureAddress , sizeof(measureAddress)) == -1)
    {
        printf("Bind failed with error code : %d" ,errno);
		close(measureSocket);
        return -1;
    }
    printf("Bind() success\n");
  
  
    if (listen(measureSocket, 5) < 0) 
    {
	    printf("listen() failed with error code : %d",errno);
		close(measureSocket);
        return -1;
    }
	
    struct sockaddr_in senderAddress;  //
    socklen_t senderAddressLen = sizeof(senderAddress);


    int iter = 0;
    time_t total = 0;
	static double totalt = 0;
    int allBytes=0;
	socklen_t len;
			int bytesReceived = 0;
    while (iter<5)
    {
    	memset(&senderAddress, 0, sizeof(senderAddress));
        senderAddressLen = sizeof(senderAddress);
        int senderSocket = accept(measureSocket, (struct sockaddr *)&senderAddress, &senderAddressLen);
    	
		if (senderSocket == -1){
		  printf("listen failed with error code : %d",errno);
		  close(senderSocket);
                 return -1;
    	}
		

		int count = 0;
		total = 0;
		double totalt = 0;
		clock_t start = clock();
        time_t t = time(0);
	    allBytes=0;
        while((bytesReceived = recv(senderSocket, buffer, sizeof(buffer), 0)) > 0 ) 
			{
	            allBytes += bytesReceived;
	        }
  
    	clock_t end = clock();
		time_t e = time(0);
		total+=e-t;
		totalt+=((double)(end-start));
		
		 if (bytesReceived<0)
	            perror("Receiving");
	        iter++;
			close(senderSocket);
	    }
		printf("Recieving avarage time =  %f\n", totalt/5.0);
		//////////Change to Reno////////////
	
		printf("Change CC o\n");
		strcpy(buffer, "reno"); 
		len = sizeof(buffer);
		if (setsockopt(measureSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, len) != 0) 
		{
			perror("setsockopt"); 
			return -1;
		}
		
		if (getsockopt(measureSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) != 0) 
		{
			perror("getsockopt"); 
			return -1; 
		}
		printf("New: %s\n", buffer); /////token from ex. example.
		
		iter = 0;	
		allBytes = 0;
		total = 0;
		totalt = 0;
		
	while (iter<5)
    {
    	memset(&senderAddress, 0, sizeof(senderAddress));
        senderAddressLen = sizeof(senderAddress);
        int senderSocket = accept(measureSocket, (struct sockaddr *)&senderAddress, &senderAddressLen);
    	
		if (senderSocket == -1){
		  printf("listen failed with error code : %d",errno);
		  close(senderSocket);
                 return -1;
    	}	
    	clock_t start = clock();
		time_t t = time(0);
    	
    	
		while( (bytesReceived = recv(senderSocket, buffer, sizeof(buffer), 0)) > 0 ) 
			{
	            allBytes += bytesReceived;
	        }
  
    	clock_t end = clock();
		time_t e = time(0);

		
		total+=e-t;
		totalt+=((double)(end-start));
		if (bytesReceived<0)
	            perror("Receiving");
	        iter++;
			close(senderSocket);
	    }
		printf("Recieving avarage time =  %f\n", totalt/5.0);
		
	close(measureSocket);
    return 0;
}
