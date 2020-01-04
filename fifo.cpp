// #include <stdio.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <string>
// #include <error.h>
#include <errno.h>
#include "fifo.h"

using namespace Yal;
FIFO fifo[128];

int rtf_get(int id,void* data,size_t size){
  if( !(fifo[id].file_descriptor() > 0) ){
    rtf_create(id,size);
  }

  return fifo[id].get(data,size);
}

int rtf_put(int id, void* data, size_t size){
  if( !fifo[id].file_descriptor() ){
    rtf_create(id,size);
  }

  return fifo[id].put(data,size);
}

void rtf_flush(int id){
  if( !(fifo[id].file_descriptor() > 0) ){
    return;
  }
  fifo[id].flush();
}

int rtf_ovrwr_put(int id, void* data, size_t size){
  if( !(fifo[id].file_descriptor() > 0) ){
    rtf_create(id,size);
  }

  return fifo[id].ovrwr_put( data, size );
}

int rtf_create(int id, size_t size){
  fifo[id].create("rtf",id);
  return fifo[id].open(true);//non-blocking
}

void rtf_destroy(int id){
  fifo[id].close();
}

//----------------

int FIFO::get(void* data,size_t size){

  if( !(fd > 0) ){
    return -1;
  }

  //ssize_t ret = read(fd,data,size);
  return read(fd,data,size);
  //リターン値をどうするか
  //return ret == size ? ret : 0;
}

int FIFO::put(void* data, size_t size){

  if( !(fd > 0) ){
    return -1;
  }
  ssize_t wsize = write( fd, data, size );

  if( wsize < 0 ){
    if( errno == EAGAIN ){
      flush();
      wsize = write( fd, data, size );
      //最新のデータを優先し押し出して書き込む

      //fprintf(stderr,"forced put\n");
    }
    else{
      perror("Ktl::FIFO::put : write");
      //fprintf(stderr,"%d == %d\n",errno,EAGAIN);
    }
  }

  return wsize;
}


void FIFO::flush(){//remove old data

  if( !(fd > 0) ){
    return;
  }
#define RSIZE 1024
  static char dump[RSIZE];
  
  int count = 0;
  
  while(1){
    int ret = read( fd, dump, RSIZE ); //
    if(ret < RSIZE){ //FIFOが空か全部読めた場合
      //if( ret > 0 )
      //printf("FIFO::flush : ret = %ld < %d at %d\n",ret,RSIZE,count);
      //else printf("FIFO::flush : ok\n");
      //break;
      return;
    }
    else if( count > 64 ){ //RSIZE*64 = 65536 デフォルトのバッファサイズ
      fprintf(stderr,"FIFO::flush : over 64 read\n");
      //break;
      return;
    }
    //else printf("FIFO::flush : ret = %ld at %d\n",ret,count);
  
    count++;
  }
}


int FIFO::ovrwr_put(void* data, size_t size){
  if( !(fd > 0) ){
    return -1;
  }

  flush();

  return write( fd, data, size );
}

FIFO::FIFO(){}
FIFO::FIFO(std::string name){
  path = "/tmp/" + name;
  mkfifo( path.c_str(), 0666 );
}

int FIFO::create(std::string name){
  path = "/tmp/" + name;
  
  return mkfifo( path.c_str(), 0666 );

}

int FIFO::create(std::string name, int index){//, size_t size){
  name += std::to_string(index);

  return FIFO::create(name);
}


//int FIFO::open(int flag_nonblocking){
int FIFO::open(int blocking){
  if( blocking == BLOCKING )
    fd = ::open(path.c_str(), O_RDWR );
  else
    fd = ::open(path.c_str(), O_RDWR | O_NONBLOCK);

  
  //needs to be RDWR
  
  if( fd < 0 ){ 
    fprintf(stderr,"Ktl::FIFO::create cannot open open %s\n",path.c_str());
    perror("open");
    return -1; 
  }
  return fd;  
}

void FIFO::close(){
  ::close( fd );
  fd = -1;
}

int FIFO::destroy(){
  return remove(path.c_str());
}
