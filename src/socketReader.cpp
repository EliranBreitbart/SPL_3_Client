//
// Created by eliranbr@wincs.cs.bgu.ac.il on 04/01/2022.
//

#include "../include/socketReader.h"

socketReader::socketReader(ConnectionHandler* _connectionHandler) : connectionHandler(_connectionHandler){}

void socketReader::run() {
    while (!connectionHandler->isTerminated() && 1) {
        std::string answer;
        if (!connectionHandler->getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            std::cout << "Press Enter to exit\n" << std::endl;
            connectionHandler->setTerminated(true);
            break;
        } else {
            if (answer == "10 3") {
                std::cout << "Exiting...\n" << std::endl;
                connectionHandler->setTerminated(true);
                break;
            }else{
                std::cout<<answer<<std::endl;
            }
        }
    }
}