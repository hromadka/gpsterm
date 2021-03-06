#include <CppLinuxSerial/SerialPort.hpp>
#include <cstring>
#include <stdlib.h>		// stoi
#include <locale>		// std::locale, std::tolower
#include <unistd.h>		// sleep


using namespace mn::CppLinuxSerial;

int main(int argc, char ** argv){
	std::string gpsport = "ttyUSB0";
	bool simulation_mode = false;
	int provider_id = 0;
	float version_num = 0.1;

	for(int i = 1; i < argc; i++) {
		// print all input params to confirm
		//printf("Argument %d: %s\n", (i+1), argv[i]);
	
		std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            std::cout << "'gpsterm' is a simple utility to read incoming NMEA strings from an attached serial GPS device." << std::endl << std::endl;
			std::cout << "Usage:" << std::endl;
			std::cout << "  ./gpsterm [options]" << std::endl << std::endl;
			std::cout << "Input arguments:" << std::endl;
			std::cout << "  -h              help" << std::endl;
// actually this is used by gpspubsub, not by the standalone gpsterm
//			std::cout << "  -i              sets ID to the specified integer.  Default ID is 0." << std::endl;
			std::cout << "  -p <USB_port>   specifies USB port under /dev, e.g. '-p ttyUSB1'.  Without this flag, the default port is /dev/ttyUSB0." << std::endl;
			std::cout << "  -s              controls simulation mode.  If '-s' flag is used, gpsterm reports a simulated position with random walk.  " << std::endl;
			std::cout << "  -v 				version info." << std::endl;
			std::cout << std::endl;
            return 0;
//		} else if ((arg == "-i") || (arg == "--id")) {
//            if (i + 1 < argc) { 
//                provider_id = strtol(argv[i+1], NULL, 10);
//				i++; 
//            } else { 
//                std::cerr << "--id option requires a value!" << std::endl;
//                return 1;
//            }  
        } else if ((arg == "-p") || (arg == "--port")) {
            if (i + 1 < argc) { 
                gpsport = argv[i+1];
				i++; 
            } else { 
                std::cerr << "--port option requires a value!" << std::endl;
                return 1;
            }  
		} else if ((arg == "-s") || (arg == "--sim")) {
			simulation_mode = true; // technically, only need "-s", without another arg, unless want to pass starting coords this way.
		} else if ((arg == "-v") || (arg == "--version")) {
			std::cout << version_num << std::endl;
			return 0;
        } else {
            std::cerr << "unrecognized input parameter: " << argv[i] << std::endl;
			return 1;
        }
				
	}

	std::cout << "id = " << provider_id << std::endl;
	if (simulation_mode) {
		std::cout << "gpsport: simulation mode = " << simulation_mode << std::endl;
	} else {
		std::cout << "gpsport: using port /dev/" + gpsport << std::endl;
 	}


	if (simulation_mode) {
		while (1) {
			//"$GPGGA,165125.568,,,,,0,00,,,M,0.0,M,,0000*59"
			std::string lat = "33.70024";
			std::string lon = "112.09462";
			std::string prefix = "$GPGGA,165125.568,";
			std::string suffix = "333,00,,,M,0.0,M,,0000*99";
			std::string rando = std::to_string(rand() % 10);
			std::string sentence = prefix + lat + rando + ",N," + lon + rando + ",W," + suffix;
			std::cout << sentence << std::endl;
			
			sleep(1);
		}

	} else {
		// Create serial port object 
		SerialPort serialPort("/dev/" + gpsport, BaudRate::B_4800, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
		serialPort.SetTimeout(-1); // Block when reading until any data is received
		
		std::cout << "opening serial port" << std::endl;
		
		serialPort.Open();

		std::cout << "waiting for first endline character" << std::endl;
		
		int length = 0;
		char char_array[1028];
		// Read some data back (will block until at least 1 byte is received due to the SetTimeout(-1) call above).
		// TODO: add timer to allow graceful exit from while(1)
		// TODO: use udev library to detect device removal
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
					char_array[0] = '\0';  // fix issue with printing newline with every char by clearing buffer
				}
			}
			
			std::cout << readData;
		}
		
		// Close the serial port
		serialPort.Close();

	}

}