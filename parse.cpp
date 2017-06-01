#include "lispinclude.h"

inline void pushStr(bool & inputhandled, expression * parent, char * data, const std::size_t i, const std::size_t lastpos, int type){
  std::size_t tmp;
  char * p;
  expression * exp;
  if(!inputhandled){
    tmp = (i-lastpos);
    p = data + lastpos;
    exp = makeString(p, tmp);
    exp->type = type;
    /*printStr(exp);
      std::cout << std::endl;*/
    addBackElem(parent->data.list, (void *)exp);
  }
  inputhandled = true;
}

std::size_t seekEndOfStr(char * str){
  char * p = str, c;
  while((c = *(p++)) && c != '\"'){
    if(c == '\\' && *p)
      ++p;
  }
  return p-str;
}

inline void pushInt(expression * parent, char * data, std::size_t & i){
  char * p = data, c;
  ++p;
  while((c = *(p++)) && c >= '0' && c <= '9');
  int len = p - data - 1;
  addBackElem(parent->data.list, (void *)makeInt(data, len));
  i += len - 1;
  //std::cout << "&" << data << "&" << std::endl;
}

expression * parseList(char * data, std::size_t size){
  expression * parent = new expression();
  expression * tmp;
  int type = VAR_EXP;
  std::stack <expression *> parents;
  std::size_t lastpos = 0;
  bool inputhandled = true;
  makeList(parent);
  for(std::size_t i = 0; i < size; ++i){
    if(data[i] == '('){
      pushStr(inputhandled, parent, data, i, lastpos, type);
      parents.push(parent);
      tmp = new expression();
      addBackElem(parent->data.list, (void *)tmp);
      parent = tmp;
      makeList(parent);
    }else if(data[i] == ')'){
      pushStr(inputhandled, parent, data, i, lastpos, type);
      parent = parents.top();
      parents.pop();
    }else if(data[i] == ' ' || data[i] == '\n' || data[i] == '\t'){
      pushStr(inputhandled, parent, data, i, lastpos, type);
    }else if(inputhandled){
      inputhandled = false;
      lastpos = i;
      if(data[i] == '"'){
	type = STR_EXP;
	++lastpos;
	++i;
	i = i + seekEndOfStr(data + i) - 1;
	pushStr(inputhandled, parent, data, i, lastpos, type);
      }else if ((data[i] >= '0' && data[i] <= '9') || (data[i] == '-' && i + 1 < size && (data[i + 1] >= '0' && data[i + 1] <= '9'))){
	type = CONST_EXP;
	pushInt(parent, data + i, i);
	inputhandled = true;
      }else{
	type = VAR_EXP;
      }
    }    
  }

  if(!parents.empty()){
    deleteExpression(parent);
    return NULL;
  }else{
    return parent;
  }
}
