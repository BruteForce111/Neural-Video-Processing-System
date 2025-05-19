

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <cuda_runtime.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "NvInfer.h"

void checkCudaErrors(cudaError_t err) {
    if (err != cudaSuccess) throw std::runtime_error(cudaGetErrorName(err));
}

// Logger for TensorRT info/warning/errors
class Logger : public nvinfer1::ILogger {
public:
    Logger(Severity severity = Severity::kWARNING) : reportableSeverity(severity) {}

    void log(Severity severity, const char* msg) noexcept override {
        // suppress messages with severity enum value greater than the reportable
        if (severity > reportableSeverity) return;

        switch (severity) {
        case Severity::kINTERNAL_ERROR:
            std::cerr << "INTERNAL_ERROR: ";
            break;
        case Severity::kERROR:
            std::cerr << "ERROR: ";
            break;
        case Severity::kWARNING:
            std::cerr << "WARNING: ";
            break;
        case Severity::kINFO:
            std::cerr << "INFO: ";
            break;
        default:
            std::cerr << "UNKNOWN: ";
            break;
        }
        std::cerr << msg << std::endl;
    }

    Severity reportableSeverity;
};
template <typename T>
struct TrtDeleter {
    void operator()(T* p) noexcept {
        if (p != nullptr) delete p;
    }
};

template <typename T>
struct CuMemDeleter {
    void operator()(T* p) noexcept { checkCudaErrors(cudaFree(p)); }
};

template <typename T>
std::unique_ptr<T, CuMemDeleter<T>> mallocCudaMem(size_t nbElems) {
    T* ptr = nullptr;
    checkCudaErrors(cudaMalloc((void**)&ptr, sizeof(T) * nbElems));
    return std::unique_ptr<T, CuMemDeleter<T>>{ptr};
}

struct EventDeleter {
    void operator()(CUevent_st* event) noexcept { checkCudaErrors(cudaEventDestroy(event)); }
};
struct StreamDeleter {
    void operator()(CUstream_st* stream) noexcept { checkCudaErrors(cudaStreamDestroy(stream)); }
};

std::unique_ptr<CUevent_st, EventDeleter> makeCudaEvent(int flags) {
    cudaEvent_t event;
    checkCudaErrors(cudaEventCreateWithFlags(&event, flags));
    return std::unique_ptr<CUevent_st, EventDeleter>{event};
}

std::unique_ptr<CUstream_st, StreamDeleter> makeCudaStream(int flags, int priority) {
    cudaStream_t stream;
    checkCudaErrors(cudaStreamCreateWithPriority(&stream, flags, priority));
    return std::unique_ptr<CUstream_st, StreamDeleter>{stream};
}



#endif
