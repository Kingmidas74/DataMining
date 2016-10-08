SOURCE_FOLDER	:= ./src
DATA_FOLDER		:= ./data
ENTRY_POINT		:= $(SOURCE_FOLDER)/Source.cpp
TARGET			:= ./build/
BASE_OPTIONS	:= -std=c++14 -qopenmp -O3 -vec-report=5 -I $(SOURCE_FOLDER) -Who-unknown-pragmas -Wall -Warray-bounds -Wchar-subscripts -Wcomment -Wenum-compare -Wformat -Wuninitialized -Wmaybe-uninitialized -Wmain -Wnarrowing -Wnonnull -Wparentheses -Wpointer-sign -Wreorder -Wreturn-type -Wsign-compare -Wsequence-point -Wtrigraphs -Wunused-function -Wunused-but-set-variable -Wunused-variable -Wwrite-strings

all:
	mkdir -p $(TARGET)
	icpc $(ENTRY_POINT) -o $(TARGET)ParallelClusteringPhi.mic -mmic $(BASE_OPTIONS)
	icpc $(ENTRY_POINT) -o $(TARGET)ParallelClusteringPhi $(BASE_OPTIONS)
	cp -a -n $(DATA_FOLDER)/. $(TARGET)
