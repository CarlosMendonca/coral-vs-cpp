#include <iostream>

#include <edgetpu/edgetpu.h>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <flatbuffers/flatbuffers.h>

int main() {
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


	const std::string path_to_model = ".\mobilenet_v1_1.0_224_quant_edgetpu.tflite";

	std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(path_to_model.c_str());

	if (model == nullptr) {
		std::cerr
			<< "ERROR: Fail to build FlatBufferModel from file '"
			<< path_to_model
			<< "'"
			<< std::endl;
		std::abort();
	}

	return 0;
}