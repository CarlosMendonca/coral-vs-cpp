#include <iostream>

#include <edgetpu/edgetpu.h>

int main() {
	const std::vector<edgetpu::EdgeTpuManager::DeviceEnumerationRecord> &connectedEdgeTpus
		= edgetpu::EdgeTpuManager::GetSingleton()->EnumerateEdgeTpu();

	std::cout << "Edge TPUs in the system: " << connectedEdgeTpus.size() << std::endl;
	for (edgetpu::EdgeTpuManager::DeviceEnumerationRecord edgetpu : connectedEdgeTpus)
		std::cout
			<< "- "
			<< (edgetpu.type == edgetpu::DeviceType::kApexUsb ? "[USB] " : "[PCI] ")
			<< edgetpu.path.c_str()
			<< std::endl;

	return 0;
}