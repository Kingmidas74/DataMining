# Names
PROJECT_NAME	:= DataMining
# End Names

#Compiler
CC 				:= g++
VAR_COMPILE 	:= HOST_COMPILE
ifeq ($(ACC),GPU)
CC 				:= nvcc
VAR_COMPILE 	:= CUDA_COMPILE
endif
ifeq ($(ACC),MIC)
CC 				:= icpc
VAR_COMPILE 	:= MIC_COMPILE
endif

# Folders
SOURCE_FOLDER	:= ./src
DATA_FOLDER		:= ./data
SCRIPT_FOLDER	:= ./scripts
TARGET_FOLDER	:= ./bin
OBJECT_FOLDER	:= ./obj
PCH_FOLDER		:= $(OBJECT_FOLDER)/pch
REPORTS_FOLDER	:= ./reports
# End Folders

#Files
ENTRY_POINT	:= $(SOURCE_FOLDER)/Source.cpp
PCH_FILE	:= $(PCH_FOLDER)/$(PROJECT_NAME)
#End Files



# ICPC Options
GCC_BASE_OPTIONS	= -std=c++17 -fopenmp -I ./src
GCC_OPTIMIZATIONS	= -O3 -Ofast -march=skylake #-ipo -ipo-c -parallel -no-prec-div -align
GCC_WARNING_OPTIONS= -Wall -Warray-bounds -Wchar-subscripts -Wcomment -Wenum-compare -Wformat -Wuninitialized -Wmaybe-uninitialized -Wmain -Wnarrowing -Wnonnull -Wparentheses -Wpointer-sign -Wreorder -Wreturn-type -Wsign-compare -Wsequence-point -Wtrigraphs -Wunused-function -Wunused-but-set-variable -Wunused-variable -Wwrite-strings

GCC_SHORT_OPTIONS	= $(GCC_BASE_OPTIONS) $(GCC_OPTIMIZATIONS) #$(GCC_WARNING_OPTIONS)


# ICPC Options
ICPC_BASE_OPTIONS	= -std=c++17 -qopenmp -qopt-report-phase=all -I $(SOURCE_FOLDER)
ICPC_OPTIMIZATIONS	= -ansi-alias -O3 -ipo -ipo-c -parallel -no-prec-div -align -fast -m64
ICPC_WARNING_OPTIONS= -Wall -Warray-bounds -Wchar-subscripts -Wcomment -Wenum-compare -Wformat -Wuninitialized -Wmaybe-uninitialized -Wmain -Wnarrowing -Wnonnull -Wparentheses -Wpointer-sign -Wreorder -Wreturn-type -Wsign-compare -Wsequence-point -Wtrigraphs -Wunused-function -Wunused-but-set-variable -Wunused-variable -Wwrite-strings

ICPC_PHI_OPTIONS	= -mmic -qopt-report-file=$(REPORTS_FOLDER)/$@.phi.optrpt
ICPC_XEON_OPTIONS	= -xSSE4.2 -qopt-report-file=$(REPORTS_FOLDER)/$@.node.optrpt

ICPC_SHORT_OPTIONS	= $(ICPC_BASE_OPTIONS) $(ICPC_WARNING_OPTIONS) $(ICPC_OPTIMIZATIONS)

CXXFLAGS := $(GCC_SHORT_OPTIONS)
ifeq ($(ACC),MIC)
CXXFLAGS := $(ICPC_SHORT_OPTIONS) $(ICPC_XEON_OPTIONS)
endif
# End Options

OBJECT_FILES := $(addprefix $(OBJECT_FOLDER)/, $(addsuffix .o, algorithms functions io $(PROJECT_NAME)))

OBJ_COMP = $(CC) $< -o $(OBJECT_FOLDER)/$@.o $(CXXFLAGS)

all: folders $(PROJECT_NAME) files
	$(CC) $(OBJECT_FOLDER)/$(word 2,$^).o -o $(TARGET_FOLDER)/$(PROJECT_NAME) $(CXXFLAGS)

$(PROJECT_NAME): algorithms functions io
	$(CC) -c $(ENTRY_POINT) -o $(OBJECT_FOLDER)/$@.o $(CXXFLAGS)

algorithms: $(SOURCE_FOLDER)/algorithms/main.h clustering evaluation metric preprocessing
	$(OBJ_COMP)

clustering: $(SOURCE_FOLDER)/algorithms/clustering/main.h c-algorithms k-algorithms
	$(OBJ_COMP)

c-algorithms: $(SOURCE_FOLDER)/algorithms/clustering/c-algorithms/main.h c-means
	$(OBJ_COMP)

k-algorithms: $(SOURCE_FOLDER)/algorithms/clustering/k-algorithms/main.h k-means
	$(OBJ_COMP)

c-means: $(SOURCE_FOLDER)/algorithms/clustering/c-algorithms/c-means/main.h preprocessing evaluation metric functions
	$(OBJ_COMP)

k-means: $(SOURCE_FOLDER)/algorithms/clustering/k-algorithms/k-means/main.h preprocessing evaluation metric functions
	$(OBJ_COMP)

preprocessing: $(SOURCE_FOLDER)/algorithms/preprocessing/main.h normalization
	$(OBJ_COMP)

normalization: $(SOURCE_FOLDER)/algorithms/preprocessing/normalization/main.h metric
	$(OBJ_COMP)

evaluation: $(SOURCE_FOLDER)/algorithms/evaluation/main.h metric
	$(OBJ_COMP)

metric: $(SOURCE_FOLDER)/algorithms/metric/main.h functions
	$(OBJ_COMP)

io: $(SOURCE_FOLDER)/io/main.h
	$(OBJ_COMP)

functions: $(SOURCE_FOLDER)/functions/main.h
	$(OBJ_COMP)

clean:
	rm -r $(OBJECT_FOLDER)
	rm -r $(TARGET_FOLDER)

folders:
	mkdir -p $(TARGET_FOLDER)
	mkdir -p $(REPORTS_FOLDER)
	mkdir -p $(OBJECT_FOLDER)

files:
	cp -a -n $(DATA_FOLDER)/. $(TARGET_FOLDER)
