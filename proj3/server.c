#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>


// define contants
#define BUFMAX  2048
#define PORTNUM 28062
#define MAX     1024               


// define the struct of data in databse 
struct record
{
    int	  acctnum;
    char  name[20];
    float value;
    int age;
};

// setting up signal catcher for child
void signal_catcher(int the_sig){
  wait(0);                     
}


int main(int argc,char *argv[])
{
  // SETTING UP TCP AND LEAVE IT OPEN TO RECEIVE
  
  // define sockets and length of client 
  int		orig_sock, new_sock;
  socklen_t	clnt_len;
  
  // define client address and server address for TCP  as well as socket length
  struct	sockaddr_in	clnt_adr, serv_adr; // client and  server addresses
  socklen_t	serv_add_len;
  
  // len is the length of input data
  int	len, i; 
  
  // signal catcher if when child terminates--> if error happens it means that child is not terminated and it is in zombie state
  if (signal(SIGCHLD , signal_catcher) == SIG_ERR) {
    perror("SIGCHLD"); 
    return 1;
  }
  
  // setup socket
  if ((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("generate error"); 
    return 2;
  }
  
  serv_adr.sin_family      = AF_INET;     // Set address type: receive : Meaning, AF_INET refers to addresses from the internet, IP addresses specifically
  serv_adr.sin_addr.s_addr = INADDR_ANY;  // Any interface: receive from any incoming address
  serv_adr.sin_port        = 0;    // setup the port to 0 so system would assign value
  
  // bind the original socket with server address  
  if (bind( orig_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0){
    close(orig_sock);
    perror("bind error"); return 3;
  }
  
  // get the socketname and put into orig_sock to be truned to service map   
  serv_add_len = sizeof(serv_adr);
  getsockname(orig_sock, (struct sockaddr *)&serv_adr, &serv_add_len);

    
  // TCP task 1 done it is set up now
  //-----------------------------------------------------------------------------------------
  // start UDP: 
  

  // set the broadcast address
  char ADDR_NUM[] = "10.219.255.255";

  // set the command to service map
  char PUT[] = "PUT CISBANK";
  
  // setup a message which contans the data sent to service map
  char MSG[40];
  
  // contactinate all data sent from server to service map through UDP
  sprintf(MSG, "%s %d", PUT,  ntohs(serv_adr.sin_port));
  
  // initialize socket sk and buffer buf
  int sk;
  unsigned char buf[BUFMAX];
  
  // initilize the socket address of the remote which is service map in this case
  struct sockaddr_in remote;
  
  // initilize hostent which is  Structure that describes an Internet host
  struct hostent *hp;

  // Create an Internet domain datagram socket 
  sk = socket(AF_INET,SOCK_DGRAM,0);
  remote.sin_family = AF_INET;

  //Get the remote machine address from its symbolic name given in the command line argument 
  //remote.sin_addr.s_addr = inet_addr(ADDR_NUM); 

  // get the remote port number from the command line 
  remote.sin_port = ntohs(28062);

  // setsockopt is required on Linux, but not on Solaris 
  setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remote,sizeof(remote));
  
  // send the mesage to service map through UDP
  sendto(sk,MSG,strlen(MSG)+1,0,(struct sockaddr *)&remote,sizeof(remote));
  
  // get the returned length and data from service map which should contain OK
  socklen_t addrlen;
  recvfrom(sk, &buf, BUFMAX, 0, (struct sockaddr *)&remote, &addrlen);
  
  // intilize new buffer which holds the value of port and the print 
  char ip_buff[100];
  inet_ntop(AF_INET, &(remote.sin_addr), ip_buff, 100);
  
  if (strcmp(buf, "OK") == 0){
      printf("Registration OK from %s\n", ip_buff);
  } 
  
  // close UDP socket once communication is done with service map
  close(sk);
  
  // UDP is done 
  // -------------------------------------------------------------------------------------------
  // get back to TCP now lets communicate with client
  
  // setup max listening to 5 machines at the same time, if more, close socket and return error
  if (listen(orig_sock, 5) < 0 ) {               
    close (orig_sock);
    perror("listen error"); 
    return 4;
  }
  
  // while loop to do TCP data communications
  do {
    clnt_len = sizeof(clnt_adr);
    // if there is a coming tcp request, upon acceptance make new socket
    if ((new_sock = accept( orig_sock, (struct sockaddr *) &clnt_adr, &clnt_len)) < 0) {
      close(orig_sock);
      perror("accept error"); 
      return 5;
    }
    
    // setup buffer to hold port number and then print port number
    char ip_buff_tcp[100];
    inet_ntop(AF_INET, &(clnt_adr.sin_addr), ip_buff_tcp, 100);
    printf("Service requested from %s\n", ip_buff_tcp);

    // for a child to handle multiple connection
    if ( fork( ) == 0 ) {                       
      // close original one to avoid zombie tcp or tco connection leak
      close(orig_sock);
      while ( (len=recv(new_sock, buf, MAX, 0)) > 0 ){
      
        // setup three inputs for incoming data. for query only two of them are used
        uint32_t recv1;
        uint32_t recv2;
        uint32_t recv3;
        
        // do the byte shifting to generate the 32 bit (4-byte) comand
        recv1 = buf[0] + (buf[1]<<8) + (buf[2]<<16) + (buf[3]<<24)  ;
        recv1 = recv1 >> 16;
        // big endian to little endian
        recv1 = ntohs(recv1);
        
        // if command is about update
        if (recv1==1001){
          
          // do bit shifting for the seconf chunk
          recv2 = buf[4] + (buf[5]<<8) + (buf[6]<<16) + (buf[7]<<24)  ;
          recv2 = recv2 >> 16;
          recv2 = ntohs(recv2);
          
          // get the floating value and convert to float
          recv3 = buf[8] + (buf[9]<<8) + (buf[10]<<16) + (buf[11]<<24)  ;
          recv3 = ntohs(recv3);
          recv3 = recv3<<16;
          uint32_t num = recv3;
          float f;
          f = *((float*)&num);
          
          // set upi error flag for data not found. it sets to 1 once found
          int flag = 0;
          
          // 32 byte data counter and struct
          int counter;
          struct record my_record;
          
          // open database file
          int ptr_myfile = open("db18",O_RDWR);
          if (!ptr_myfile){
            printf("Unable to open file!\n");
          }
          
          // initilize the sending buffer
          char buff_send [100];
          
          // counting from bottom to top of databse lookingf for the larget data
          for ( counter=15; counter >= 0; counter--){
          
            // set lseek to the beggining and read the whole 32 byte data into my_record
            lseek(ptr_myfile,sizeof(struct record)*counter,SEEK_SET);
            read(ptr_myfile, &my_record,sizeof(struct record));
            
            // if the account number matches with the second part of command from client
            if (my_record.acctnum==recv2){
              
              // set the flag to 1 to let know that data found
              flag = 1;
              
              // add the input float value "f" to the record.value              
              my_record.value = my_record.value + f;
              
              // set lseek to the beggining of my_record.value to update the database
              lseek(ptr_myfile,sizeof(struct record)*counter+24,SEEK_SET);              
              
              // lock to avoid anybody esle make change, error if lock not available
              if (lockf(ptr_myfile, F_LOCK, sizeof(my_record.value))==-1){
                  printf("fail\n");
              }
              
              // white to the location
              write(ptr_myfile , &my_record.value , sizeof(my_record.value) );
              
              // release lock. error if lock no released             
              if (lockf(ptr_myfile, F_ULOCK, sizeof(my_record.value))==-1){
                  printf("fail\n");
              }
              
              // lseek to the beginning of the the same row of data and read again and save into my_record
              lseek(ptr_myfile,sizeof(struct record)*counter,SEEK_SET);              
              read(ptr_myfile, &my_record,sizeof(struct record));
              
              // concatinate data to send back to client
              sprintf(buff_send, "%s %d %.1f                  ", my_record.name, my_record.acctnum, my_record.value);
              
              // send the buffer back to client
              send(new_sock, buff_send, 32, 0);      
            }
          }
          if (flag == 0){
              // if data not found send error back to client
              char ret_msg[] = "Inquiry not found";
              sprintf(buff_send, "%s                                   ", ret_msg);
              send(new_sock, buff_send, 32, 0);        
          }
          
          // close file descriptor
          close(ptr_myfile);
        }
        
        
        // the query is the same as update without any changing 
        if (recv1==1000){
                      
          recv2 = buf[4] + (buf[5]<<8) + (buf[6]<<16) + (buf[7]<<24)  ;
          recv2 = recv2 >> 16;
          recv2 = ntohs(recv2);

          int counter;
          struct record my_record;
          
          int ptr_myfile = open("db18",O_RDWR);
          
          if (!ptr_myfile){
            printf("Unable to open file!\n");
          }
          
          int flag = 0;
          char buff_send [100];
          
          for ( counter=15; counter >= 0; counter--){

            lseek(ptr_myfile,sizeof(struct record)*counter,SEEK_SET);
            read(ptr_myfile, &my_record,sizeof(struct record));

            if (my_record.acctnum==recv2){
              flag = 1;
              sprintf(buff_send, "%s %d %.1f                  ", my_record.name, my_record.acctnum, my_record.value);
              send(new_sock, buff_send, 32, 0);               
            }
          }
          
          if (flag == 0){
              char ret_msg[] = "Inquiry not found";
              sprintf(buff_send, "%s                                   ", ret_msg);
              send(new_sock, buff_send, 32, 0);               
          }
          
          close(ptr_myfile);
        }
        // close child socket
        close(new_sock);                           
        return 0;
      }
    } else
      // close socket in parent
      close(new_sock);                           
  } while( 1 );                               
  return 0;

}





