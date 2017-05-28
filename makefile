CXX      = g++
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CXX_FILE = interp.cpp
INCPATH=include
TARGET   = $(patsubst %.cpp,%,$(CXX_FILE))
CXXFLAGS = -g -Wall -Werror -std=c++11 -pedantic-errors -fmessage-length=0 -I $(INCPATH)

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
fclean:
	make clean
re:
	make fclean all
run:
	make re
	./$(TARGET)
