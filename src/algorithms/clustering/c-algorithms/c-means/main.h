#include "./PreCompiled.h"

#include "./functions/main.h"
#include "./io/main.h"
#include "./algorithms/metric/main.h"
#include "./algorithms/preprocessing/normalization/main.h"

#include "../../Parameters.hpp"
#include "../../ClusterParameters.hpp"
#include "../../Clustering.hpp"


#ifdef CUDA_COMPILE
    #include "./FuzzyCMeans.cuh"
#else
    #include "./FuzzyCMeans.hpp"
#endif