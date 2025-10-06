/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * students : Sahil & Teereza
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int currentTime = 0; // Tracks the current time starting from 0 in ms
    int saveTime = 10; // Time to save context
    int isrTime = 40; // Time taken for one ISR body execution


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if (activity == "CPU") { // if the acitvity is CPU, Ex: SYSCALL or END_IO
            execution += std::to_string(currentTime) + ", " + std::to_string(duration_intr) + ", CPU burst.\n";
            currentTime += duration_intr;
        } 
        else { // if the activity is not CPU
            auto [boilerplate, boilerplate_end_time] = intr_boilerplate(currentTime, deviceNum, saveTime, vectors);
            execution += boilerplate;

            currentTime = boilerplate_end_time;
            execution += std::to_string(currentTime) + ", " + std::to_string(isrTime) + ", call device driver.\n";
            currentTime += isrTime;

            int deviceDelay = delays.at(deviceNum);
            currentTime += (deviceDelay - isrTime);
            execution += std::to_string(currentTime) + ", 1, IRET. \n"; //
            currentTime += 1;
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
