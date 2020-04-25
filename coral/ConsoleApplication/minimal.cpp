#pragma comment(lib, "edgetpu.dll.if.lib")
#pragma comment(lib, "tensorflowlite.dll.if.lib")

#include <edgetpu/edgetpu.h>

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>

#include <iostream>

#include "bitmap_utils.h"
#include "model_utils.h"
#include "win_utils.h"

int main() {

	// Find Edge TPUs on system
	const std::vector<edgetpu::EdgeTpuManager::DeviceEnumerationRecord> &connectedEdgeTpus
		= edgetpu::EdgeTpuManager::GetSingleton()->EnumerateEdgeTpu();

	std::cout
		<< "Edge TPUs in the system: "
		<< connectedEdgeTpus.size()
		<< std::endl;

	if (connectedEdgeTpus.size() == 0) {
		std::cerr
			<< "ERROR: No Edge TPUs found."
			<< std::endl;
		std::abort();
	}

	for (edgetpu::EdgeTpuManager::DeviceEnumerationRecord edgetpu : connectedEdgeTpus)
		std::cout
			<< "- "
			<< (edgetpu.type == edgetpu::DeviceType::kApexUsb ? "[USB] " : "[PCI] ")
			<< edgetpu.path.c_str()
			<< std::endl;

	// Read model
	const std::string path_to_model = coral::GetWorkingDir() + "mobilenet_v1_1.0_224_quant_edgetpu.tflite";

	std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(path_to_model.c_str());

	if (model == nullptr) {
		std::cerr
			<< "ERROR: Fail to build FlatBufferModel from file '"
			<< path_to_model
			<< "'."
			<< std::endl;
		std::abort();
	}

	// Read input image
	const std::string resized_image_path = coral::GetWorkingDir() + "resized_cat.bmp";

	int width, height, channels;
	const std::vector<uint8_t>& input = coral::read_bmp(resized_image_path, &width, &height, &channels);


	std::shared_ptr<edgetpu::EdgeTpuContext> edgetpu_context 
		= edgetpu::EdgeTpuManager::GetSingleton()->OpenDevice();

	std::unique_ptr<tflite::Interpreter> interpreter
		= coral::BuildEdgeTpuInterpreter(*model, edgetpu_context.get());

	const auto& result = coral::RunInference(input, interpreter.get());
	
	// Print inference result
	auto it = std::max_element(result.begin(), result.end());
	
	std::cout
		<< "[Image analysis] max value index: "
		<< std::distance(result.begin(), it)
		<< " value: "
		<< *it
		<< std::endl;

	return 0;
}