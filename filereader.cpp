#include "filereader.h"

char * readFile(char * file, std::size_t & size){
  std::ifstream ifs(file);
  std::filebuf* pbuf = ifs.rdbuf();

  // get file size using buffer's members
  size = pbuf->pubseekoff (0,ifs.end,ifs.in);
  pbuf->pubseekpos (0,ifs.in);

  // allocate memory to contain file data
  char* buffer=new char[size];
  
  // get file data
  pbuf->sgetn (buffer,size);
  
  ifs.close();
  return buffer;
}
