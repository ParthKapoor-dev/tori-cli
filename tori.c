/* TORI.C  */

#include "header.h"

int main( int argc , char * argv[]){
  
  char * host;
  int port, soc;
  struct sockaddr_in socket_address;

  if(argc < 3){
    fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
    return -1;
  }

  host = argv[1];
  port = atoi(argv[2]);
  
  soc = socket(AF_INET, SOCK_STREAM,0);
  if(soc < 0){
    perror("Socket");
    return -1;
  }
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons(PORT);
  socket_address.sin_addr.s_addr = inet_addr(PROXY);

  if(connect(soc , (struct sockaddr*)&socket_address , sizeof(socket_address))){
    perror("Connect Failed");
    return -1;
  }

  printf("Connected to proxy \n");
  close(soc);

  return 0;
}
