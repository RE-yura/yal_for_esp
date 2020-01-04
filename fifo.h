#ifndef __YAL_FIFO__
#define __YAL_FIFO__

#include <string>

namespace Yal{

class FIFO{
 public:
  enum{
    BLOCKING,
    NON_BLOCKING
  };

  FIFO();
  FIFO(std::string name);
  int get(void* data,size_t size);
  void flush();
  int put( void* data, size_t size);
  int ovrwr_put( void* data, size_t size);
  
  int create(std::string name, int index);
  int create(std::string name);
  
  int destroy();
  int open(int blocking=BLOCKING);
  void close();
  int file_descriptor(){ return fd; }
  
 private:
  std::string path;
  int fd =0;
};
}


int rtf_get(int id,void* data,size_t size);
void rtf_flush(int id);
int rtf_put(int id, void* data, size_t size);
int rtf_ovrwr_put(int id, void* data, size_t size);

int rtf_create(int id, size_t size);
void rtf_destroy(int id);

#endif
