main : TextClassification.o PreProcess.o utils.o
	g++ TextClassification.o PreProcess.o utils.o -o main
	
TextClassfication.o : TextClassification.cpp PreProcess.h
	g++ -c TextClassification.cpp

PreProcess.o : PreProcess.cpp 
	g++ -c PreProcess.cpp
utils.o : utils/utils.cpp 
	g++ -c utils/utils.cpp

clean : 
	rm *.o
