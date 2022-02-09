#include <CppLinuxSerial/SerialPort.hpp>
#include <cstring>


using namespace mn::CppLinuxSerial;

int main() {
	// Create serial port object 
	SerialPort serialPort("/dev/ttyUSB0", BaudRate::B_4800, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
	serialPort.SetTimeout(-1); // Block when reading until any data is received
	
	std::cout << "opening serial port" << std::endl;
	
	serialPort.Open();

	
	std::cout << "waiting for first endline character" << std::endl;
	
	int length = 0;
	char char_array[1028];
	// Read some data back (will block until at least 1 byte is received due to the SetTimeout(-1) call above).
	// TODO: add timer to allow graceful exit from while(1)
	while (1) {
		std::string readData;
		serialPort.Read(readData);

		strcpy(char_array, readData.c_str());
 		length = sizeof(readData);
		for (int i = 0; i < length; i++) {
        		if (char_array[i] == '\n') {
        			std::cout << std::endl;
        		}
        	}
		
		std::cout << readData;
	}
	
	// Close the serial port
	serialPort.Close();
}