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





static time_t total = 0;
static double totalt = 0;
static int count = 0;

int main(int argc, char const *argv[]) 
{ 
    	int serverSocket = -1;
    	struct sockaddr_in serverAddress, clientAddress;
    	int enableReuse = 1;
    	socklen_t clientAddressLen = sizeof(clientAddress);
    	int clientSocket = -1;
    	socklen_t len;
    
    	char buffer[65536] = {0};  
       

    	printf("Creating socket\n");
    	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    	{ 
        	printf("Could not create server socket : %d", errno);
    	} 
       
	    

    	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enableReuse, sizeof(enableReuse)) < 0) 
    	{ 
        	printf("setsockopt() failed with error code : %d", errno);
    	} 
    	
    	bzero(&serverAddress, sizeof(serverAddress)); // memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET; 
    	serverAddress.sin_addr.s_addr = INADDR_ANY; 
    	serverAddress.sin_port = htons(5060); 
       


    	if (bind(serverSocket, (struct sockaddr *)&serverAddress,  sizeof(serverAddress)) == -1) 
    	{ 
        	printf("Bind failed with error code : %d", errno);
        	close(serverSocket);
        	return -1;
    	} 
    	printf("Bind successed\n");
    	
		
		if (listen(serverSocket, 5) < 0) 
	    {							
	        printf("listen() failed with error code : %d", errno);
        	close(serverSocket);
        	return -1;
	    }
	    
	    int bytesReceived = 0;
	    int iter = 1;
	    
	    char bufcc[256];
		len = sizeof(bufcc);
		if (getsockopt(serverSocket, IPPROTO_TCP, TCP_CONGESTION, bufcc, &len) != 0) 
		{ 
		    perror("getsockopt");
		    return -1;
		}
		printf("Current CC: %s\n", bufcc);
		while(iter<=5)
		{
		memset(&clientAddress, 0, sizeof(clientAddress));
        	clientAddressLen = sizeof(clientAddress);
        	clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    		if (clientSocket == -1)
			{
        		printf("accept failed with error code : %d", errno);
	        	close(serverSocket);
        		return -1;
	        }
	        
	        
	        clock_t begin = clock();
			time_t t = time(0);
			
			int tot =0;
	        while( (bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0 ) 
			{
	            tot += bytesReceived;
	        }
			
			time_t e = time(0);
			clock_t end = clock();
			
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			count++;
			total += e-t;
			totalt += time_spent;
			printf("%d time recived in %f\n",iter,time_spent);
		


	        if (bytesReceived<0)
	            perror("Receiving");
	        iter++;
			close(clientSocket);
		}
		printf("total avarage time =  %f\n", totalt/count);




		printf("Changing CC algorithm\n");
		strcpy(buffer, "reno"); 
		len = sizeof(buffer);
		if (setsockopt(serverSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, len) != 0) 
		{
			perror("setsockopt"); 
			return -1;
		}
		
		if (getsockopt(serverSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) != 0) 
		{
			perror("getsockopt"); 
			return -1; 
		}
		
		printf("New CC: %s\n", buffer);
		
		bytesReceived = 0;
		total = 0;
		totalt = 0;
		count = 0;
		iter = 1;	
		while(iter<=5)
		{
			memset(&clientAddress, 0, sizeof(clientAddress));
        	clientAddressLen = sizeof(clientAddress);
	        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
	        if (clientSocket==-1) 
			{
        		printf("accept failed with error code : %d", errno);
	        	close(clientSocket);
        		return -1;
	        }
	        
			//printf("%ld",clock());
			clock_t begin = clock();
			time_t t = time(0);
			int tot =0;
	        while( (bytesReceived = recv(clientSocket, buffer, sizeof(buffer),0))> 0 ) 
			{
	            tot += bytesReceived;
	        }
			//bytesReceived=0;
			//printf("%ld",clock());
			clock_t end = clock();
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			count++;
			time_t e =time(0);
			total += e-t;
			totalt += time_spent;
			printf("%d time recived in %f\n",iter,time_spent);

			
	        if (bytesReceived<0)
	        	perror("can't recive file!");
	
	        iter++;
			close(clientSocket);
		}
		printf("total new avarage time =  %f\n",totalt/count);

	
	    return 0; 
} 
