#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <time.h>
#include <errno.h> 
#include <string.h> 




int main(int argc, char const *argv[]) 
{ 
    int sock = 0; 
    struct sockaddr_in measureAddr;
    
    for(int i = 1; i <=5; i ++){
    char buffer[65536] = {0};
		
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    	{ 
        	printf("\n Socket creation error \n"); 
        	return -1; 
    	} 
    	
   	memset(&measureAddr,0, sizeof(measureAddr));
    	measureAddr.sin_family = AF_INET; 
    	measureAddr.sin_port = htons(5060); 
		
        if(inet_pton(AF_INET, (const char*)"127.0.0.1", &measureAddr.sin_addr)<=0)  
    	{ 
        	printf("\inet_pton() failed.\n"); 
        	return -1; 
    	} 
    		
		

    	if (connect(sock, (struct sockaddr *)&measureAddr, sizeof(measureAddr)) < 0) 
    	{ 
        	printf("connect() failed with error code : %d \n", errno); 
        	return -1; 
    	} 
		

        

    	FILE *MyF; 
	    MyF = fopen("MyF.txt", "r");
        if(MyF == NULL){
        	perror("Error in reading file.");
        	return -1 ;
        }
        printf("Sending %d time\n",i);
		
		
		
		
        int f = 0;
        int bytesSent = 0;
        
    	while((f = fread(buffer, 1, sizeof(buffer), MyF))>0 ){ 
	   		 int bytesSent = send(sock, buffer, f, 0); //int bytesSent = send(sock, buffer, sizeof(buffer), 0);
			 if (-1 == bytesSent)
		     {
				printf("Error in sending file: %d", errno);
		     }
		     else if (0 == bytesSent)
		     {
				printf("peer has closed the TCP connection prior to send().\n");
		     }
		     else if (f > bytesSent) 
		     {
				printf("sent only %d bytes from the required %d.\n", bytesSent, f); 
		     }
		     else 
		     {

		     }
		     
   		}


		fclose(MyF);
		close(sock);
	}

	printf("New sending with Reno\n");
	for(int i = 1; i <= 5; i ++){
    	sock = 0;
	char buffer[65536] = {0}; 
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    	{ 
        	printf("\n Socket creation error \n"); 
        	return -1; 
    	} 
    	
     	memset(&measureAddr,0, sizeof(measureAddr));  
    	measureAddr.sin_family = AF_INET; 
    	measureAddr.sin_port = htons(5060); 
        if(inet_pton(AF_INET, (const char*)"127.0.0.1", &measureAddr.sin_addr)<=0)  
    	{ 
        	printf("\ninet_pton() failed \n"); 
        	return -1; 
    	}
		

	strcpy(buffer, "reno"); 
	if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, strlen(buffer)) != 0) {
	    perror("setsockopt"); 
            return -1;
	}
	socklen_t len = sizeof(buffer);
	if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &(len)) != 0) {
	    perror("getsockopt"); 
	    return -1; 
	} 

    	

    	if (connect(sock, (struct sockaddr *)&measureAddr, sizeof(measureAddr)) < 0) 
    	{ 
        	printf("\nConnection Failed \n"); 
        	return -1; 
    	} 
    		
		FILE *MyF; 
		printf("Sending %d time \n",i);
		MyF = fopen("MyF.txt", "rb");
        if(MyF == NULL){
        	perror("Error in reading file.");
        	return 2;
        }
        int f = 0;
        int bytesSent = 0;
        bzero(buffer, 65536);
    	while((f = fread(buffer, 1, sizeof(buffer), MyF))>0 ){
			 bytesSent = send(sock, buffer, f, 0);
		     if (-1 == bytesSent)
		     {
				printf("Error in sending file: %d", errno);
		     }
		     else if (0 == bytesSent)
		     {
				printf("peer has closed the TCP connection prior to send().\n");
		     }
		     else if (f > bytesSent)
		     {
				printf("sent only %d bytes from the required %d.\n", bytesSent, b);
		     }
		     else 
		     {
				
		     }
		     
   		}

		fclose(MyF);
		close(sock);
	}

	return 0; 
} 
