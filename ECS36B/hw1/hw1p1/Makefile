CXX=g++
CXXFLAGS=-Wall
EXECS=robot
all: $(EXECS)
robot: robot.o Angle.o
	$(CXX) -o $@ $^
clean:
	rm -f *.o $(EXECS)


