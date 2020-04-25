#pragma once

#include <edgetpu/edgetpu.h>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/builtin_op_data.h>
#include <tensorflow/lite/kernels/register.h>

#include <array>
#include <memory>
#include <iostream>

namespace coral {

    std::unique_ptr<tflite::Interpreter> BuildEdgeTpuInterpreter(
        const tflite::FlatBufferModel& model,
        edgetpu::EdgeTpuContext* edgetpu_context);

    std::vector<float> RunInference(
        const std::vector<uint8_t>& input_data,
        tflite::Interpreter* interpreter);

    std::array<int, 3> GetInputShape(
        const tflite::Interpreter& interpreter,
        int index);
}