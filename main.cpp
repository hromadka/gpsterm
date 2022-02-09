#include <CppLinuxSerial/SerialPort.hpp>
#include <cstring>


using namespace mn::CppLinuxSerial;

int main(int argc, char ** argv){
	std::string gpsport = "/dev/ttyUSB0";

	for(int i = 1; i < argc; i++) {
		// print all input params to confirm
		//printf("Argument %d: %s\n", (i+1), argv[i]);
		std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            std::cout << "GPSTERM is a simple utility to read incoming NMEA strings from an attached GPS over a \
			serial port that was specified using the '-p <PORT>' flag.  Without this flag, the default port is /dev/ttyUSB0." 
			<< std::endl;
            return 0;
        } else if ((arg == "-p") || (arg == "--port")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                gpsport = argv[i++]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--port option requires a value!" << std::endl;
                return 1;
            }  
        } else {
            std::cout << "unrecognized input parameter: " << argv[i] << std::endl;
        }		
	}
		
	// Create serial port object 
	SerialPort serialPort("/dev/ttyUSB1", BaudRate::B_4800, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
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

		// look for endline trigger and take action when found
 		length = sizeof(readData);
		for (int i = 0; i < length; i++) {
        	if (char_array[i] == '\n') {
				// do action(s) here
        		std::cout << std::endl;
        	}
        }
		
		std::cout << readData;
	}
	
	// Close the serial port
	serialPort.Close();
}