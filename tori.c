/* TORI.C  */

#include "header.h"

Req* request(const char* destip , const int destport){

  Req* req;

  req = malloc(reqsize);

  req->dstip= inet_addr(destip);
  req->dstport = htons(destport);
  req->vn = 4;
  req->cd = 1;
  strncpy(req->userid , USERNAME , 8);
  
  return req;
}

int main( int argc , char * argv[]){
  
  char * host;
  int port, soc;
  Req *req;
  Res *res;
  char buf[ressize];
  int success;
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
  
  req = request(host , port); 
  write(soc , req , reqsize);

  memset(buf , 0 , ressize);
  if(read(soc , buf , ressize) < 1){
    perror("read");
    free(req);
    close(soc);
    
    return -1;
  }

  res = (Res*)buf;

  success = (res->cd == 90);
  if(!success){
    fprintf(stderr , "Unable to traverse : %d\n", res->cd);
    close(soc);
    free(req);

    return -1;
  }

  printf("Successfuly Connected through proxy to ->  %s:%d \n" , host , port );

  close(soc);

  return 0;
}
