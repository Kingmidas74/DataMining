SOURCES = $(wildcard ./ParallelClusteringPhi/*.h)
TARGET := ./../../ParallelClusteringExecute/
BASE_OPTIONS := -std=c++0x -openmp -O2 -vec-report=5

all:
	mkdir -p $(TARGET)
	icpc $(SOURCES) -o $(TARGET)ParallelClusteringPhi.mic -mmic $(BASE_OPTIONS)
	icpc $(SOURCES) -o $(TARGET)ParallelClusteringPhi $(BASE_OPTIONS)
	cp -a -n ./ParallelClusteringPhi/csv/. $(TARGET)
