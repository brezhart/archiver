#include <iostream>
#include "archivate.h"
#include "dearchivate.h"
const std::string PROGRAM_NAME = "archiver";
int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Invalid instruction. Do " + PROGRAM_NAME + " -h to get more info" << std::endl;
        exit(111);
    }
    std::string command = static_cast<std::string>(*(argv + 1));
    if (command == "-c") {
        if (argc == 2) {
            std::cerr << "not enough arguments. do -h" << std::endl;
            exit(111);
        }
        std::string write_path = static_cast<std::string>(*(argv + 2));
        std::vector<std::string> files_to_archivate;
        for (int c = 3; c < argc; c++) {
            files_to_archivate.push_back(static_cast<std::string>(*(argv + c)));
        }
        Archivate(write_path, files_to_archivate);
    } else if (command == "-d") {
        std::string arc_path = static_cast<std::string>(*(argv + 2));
        Dearchivate(arc_path);
    } else if (command == "-h") {
        std::cout << "-c arc_name files... - заорхивировать files в arc_name. -d arc_name - разархивировать arc_name"
                  << std::endl;
    } else {
    }

    return 0;
}
