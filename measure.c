#include <sys/socket.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <string.h> 
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>







int main(int argc, char const *argv[]) 
{ 
    	int measureSocket = -1;
    	struct sockaddr_in serverAddress, senderAddress;
    	int enableReuse = 1;
    	socklen_t senderAddressLen = sizeof(senderAddress);
    	int senderSocket = -1;
    	socklen_t len;
    
    	char buffer[65536] = {0};  
       

    	printf("Creating socket\n");
    	if ((measureSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    	{ 
        	printf("Could not create server socket : %d", errno);
    	} 
       
	    

    	if (setsockopt(measureSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enableReuse, sizeof(enableReuse)) < 0) 
    	{ 
        	printf("setsockopt() failed with error code : %d", errno);
    	} 
    	
    	memset(&serverAddress,0, sizeof(serverAddress)); // memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET; 
    	serverAddress.sin_addr.s_addr = INADDR_ANY; 
    	serverAddress.sin_port = htons(5060); 
       


    	if (bind(measureSocket, (struct sockaddr *)&serverAddress,  sizeof(serverAddress)) == -1) 
    	{ 
        	printf("Bind failed with error code : %d", errno);
        	close(measureSocket);
        	return -1;
    	} 
    	printf("Bind successed\n");
    	
		
		if (listen(measureSocket, 5) < 0) 
	    {							
	        printf("listen() failed with error code : %d", errno);
        	close(measureSocket);
        	return -1;
	    }
	    
	    int bytesReceived = 0;
	    int iter = 1;
	    
	    char bufcc[256];
		len = sizeof(bufcc);
		if (getsockopt(measureSocket, IPPROTO_TCP, TCP_CONGESTION, bufcc, &len) != 0) 
		{ 
		    perror("getsockopt");
		    return -1;
		}
		printf("Current CC: %s\n", bufcc);
		
		double allTime = 0;
		while(iter<=5)//First 5 times send
		{
		memset(&senderAddress, 0, sizeof(senderAddress));//reset
        	senderAddressLen = sizeof(senderAddress);
        	senderSocket = accept(measureSocket, (struct sockaddr *)&senderAddress, &senderAddressLen);
    		if (senderSocket == -1)
			{
        		printf("accept failed with error code : %d", errno);
	        	close(measureSocket);
        		return -1;
	        }
	        
	        
	        clock_t start = clock();
			time_t t = time(0);
			
			int tot =0;
	        while( (bytesReceived = recv(senderSocket, buffer, sizeof(buffer), 0)) > 0 ) 
			{
	            tot += bytesReceived;
	        }
			
			time_t e = time(0);
			clock_t end = clock();
			
			double timeForSend = (double)(end - start) / CLOCKS_PER_SEC;
			allTime += timeForSend;
			
			printf("%d time recived in %f\n",iter,timeForSend);
		
	        if (bytesReceived<0)
	            perror("Receiving");
	        iter++;
			close(senderSocket);
		}
		printf("total avarage time =  %f\n", allTime/5);
		printf("Changing CC algorithm\n");
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
		
		printf("New CC: %s\n", buffer);
		bytesReceived = 0;
		allTime = 0;
		iter = 1;	
		while(iter<=5)
		{
			memset(&senderAddress, 0, sizeof(senderAddress));
        	senderAddressLen = sizeof(senderAddress);
	        senderSocket = accept(measureSocket, (struct sockaddr *)&senderAddress, &senderAddressLen);
	        if (senderSocket==-1) 
			{
        		printf("accept failed with error code : %d", errno);
	        	close(senderSocket);
        		return -1;
	        }
	        
			clock_t start = clock();
			time_t t = time(0);
			int tot =0;
	        while( (bytesReceived = recv(senderSocket, buffer, sizeof(buffer),0))> 0 ) 
			{
	            tot += bytesReceived;
	        }
			
			clock_t end = clock();
			double timeForSend = (double)(end - start) / CLOCKS_PER_SEC;
			time_t e =time(0);
			allTime += timeForSend;
			printf("%d time recived in %f\n",iter,time_spent);		
	        if (bytesReceived<0)
	        	perror("can't recive file!");
	
	        iter++;
			close(senderSocket);
		}
		printf("total new avarage time =  %f\n",allTime/5);

	    return 0; 
} 
