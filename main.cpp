#include <CppLinuxSerial/SerialPort.hpp>
#include <cstring>
#include <locale>		// std::locale, std::tolower
#include <unistd.h>		// sleep

using namespace mn::CppLinuxSerial;

int main(int argc, char ** argv){
	std::string gpsport = "/dev/ttyUSB0";
	bool simulation_mode = false;

	for(int i = 1; i < argc; i++) {
		// print all input params to confirm
		//printf("Argument %d: %s\n", (i+1), argv[i]);
		std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            std::cout << "'gpsterm' is a simple utility to read incoming NMEA strings from an attached GPS over a serial port." << std::endl;
			std::cout << "Input arguments:"  << std::endl;
			std::cout << "  -p <USB_port> specifies USB port under /dev, e.g. '-p ttyUSB1'.  Without this flag, the default port is /dev/ttyUSB0." << std::endl;
			std::cout << "  -s controls simulation mode.  If '-s' flag is used, gpsterm reports a simulated position with random walk.  " << std::endl;
            return 0;
        } else if ((arg == "-p") || (arg == "--port")) {
            if (i + 1 < argc) { 
                gpsport = argv[i++]; 
            } else { 
                std::cerr << "--port option requires a value!" << std::endl;
                return 1;
            }  
		} else if ((arg == "-s") || (arg == "--sim")) {
			simulation_mode = true; // technically, only need "-s", without another arg, unless want to pass starting coords this way.
        } else {
            std::cout << "unrecognized input parameter: " << argv[i] << std::endl;
        }		
	}
		
	std::cout << "simulation mode = " << simulation_mode << std::endl;


	if (simulation_mode) {
		while (1) {
			//"$GPGGA,165125.568,,,,,0,00,,,M,0.0,M,,0000*59"
			std::string lat = "33.70024";
			std::string lon = "112.09462";
			std::cout << "$GPGGA,165125.568,33.70024,N,112.09462,W,333,00,,,M,0.0,M,,0000*99" << std::endl;
			std::cout << rand()%10;
			sleep(1);
		}

	} else {
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
					char_array[0] = '\0';  // fix issue with printing newline with every char
				}
			}
			
			std::cout << readData;
		}
		
		// Close the serial port
		serialPort.Close();

	}

}