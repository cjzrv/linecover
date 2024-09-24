OBJS = linecover.cpp
OBJ_NAME = linecover

all : $(OBJS)
	g++ $(OBJS) -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -o $(OBJ_NAME)