# include <stdio.h>
# include <stdlib.h>
# include <string.h>


/// a sock descriptor is just an int:
int socket_descriptor;

struct sockaddr {
	unsigned short sa_family; // this will be pf_inet for internet applications 
	char sa_data[14];           // this is where the IP address and port number go 
};

struct in_addr{
	unsigned long s_addr; // 4 bytes long (AKA 32-bit) (that's how long an ip address is anyway)
};

struct sockaddr_in{
	short sin_family;        // tells you the address family (same as sa_family in sockaddr) IP/TCP 
	unsigned short sin_port; // Port number
	struct in_addr sin_addr; // IP Address.  This casts it into the strut called in_addr like above
	char sin_zero[8];
};


// NOTE THAT both the structures above are the SAME size (we can cast them into each other)
// we will actually CAST the sockaddr_in into the sockaddr  
// sockaddr    is used for things other than network communication
// sockaddr_in is used ONLY for network communication  
// There are other structures for IPv6 

void main(void){
	struct sockaddr mySock;   // remember that you must use the word "struct" for sock_addr 
	*(mySock.sa_data) = "mydata is here";
	printf("\n here it is %s\n", mySock.sa_data);
}

