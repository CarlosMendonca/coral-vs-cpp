#include <edgetpu/edgetpu.h>
#pragma comment(lib, "edgetpu.dll.if.lib")

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#pragma comment(lib, "tensorflowlite.dll.if.lib")

#include <flatbuffers/flatbuffers.h>

#include <windows.h>
#include <iostream>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

inline std::string GetWorkingDir()
{
	char path[MAX_PATH] = "";
	GetModuleFileNameA(NULL, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	PathAddBackslashA(path);
	return path;
}

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
	const std::string path_to_model = GetWorkingDir() + "mobilenet_v1_1.0_224_quant_edgetpu.tflite";

	std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(path_to_model.c_str());

	if (model == nullptr) {
		std::cerr
			<< "ERROR: Fail to build FlatBufferModel from file '"
			<< path_to_model
			<< "'."
			<< std::endl;
		std::abort();
	}

	std::shared_ptr<edgetpu::EdgeTpuContext> edgetpu_context 
		= edgetpu::EdgeTpuManager::GetSingleton()->OpenDevice();

	std::unique_ptr<tflite::Interpreter> interpreter
		= coral::BuildEdgeTpuInterpreter(*model, edgetpu_context.get());

	return 0;
}