CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRCS     = $(wildcard *.cpp)
OBJS     = $(SRCS:.cpp=.o)

shein: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) shein
