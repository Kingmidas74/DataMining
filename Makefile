SOURCE_FOLDER	:= ./src
DATA_FOLDER		:= ./data
ENTRY_POINT		:= $(SOURCE_FOLDER)/Source.cpp
TARGET			:= ./build/
BASE_OPTIONS	:= -std=c++14 -qopenmp -O3 -vec-report=5 -I $(SOURCE_FOLDER) -Who-unknown-pragmas

all:
	mkdir -p $(TARGET)
	icpc $(ENTRY_POINT) -o $(TARGET)ParallelClusteringPhi.mic -mmic $(BASE_OPTIONS)
	icpc $(ENTRY_POINT) -o $(TARGET)ParallelClusteringPhi $(BASE_OPTIONS)
	cp -a -n $(DATA_FOLDER)/. $(TARGET)
