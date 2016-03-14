main : TextClassification.o PreProcess.o
	g++ TextClassification.o PreProcess.o -o main
	
TextClassfication.o : TextClassification.cpp PreProcess.h
	g++ -c TextClassification.cpp

PreProcess.o : PreProcess.cpp 
	g++ -c PreProcess.cpp

clean : 
	rm *.o