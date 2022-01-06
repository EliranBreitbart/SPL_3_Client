//
// Created by eliranbr@wincs.cs.bgu.ac.il on 04/01/2022.
//

#include "../include/socketReader.h"

socketReader::socketReader(ConnectionHandler* _connectionHandler, keyboardReader _keyboard) : connectionHandler(_connectionHandler),keyboard(_keyboard){}

void socketReader::run() {
    while (!connectionHandler->isTerminated() && 1) {
        std::string answer;
        if (!connectionHandler->getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            std::cout << "Press Enter to exit\n" << std::endl;
            connectionHandler->setTerminated(true);
            break;
        } else {
            if (answer == "ACK 3") {
                std::cout << "Exiting... Press Enter to Close Program\n" << std::endl;
                connectionHandler->setTerminated(true);
                break;
            }else if (answer == "ERROR 3"){
                keyboard.setCloseMaybe(false);
                std::cout<<answer<<std::endl;
            } else {
                std::cout<<answer<<std::endl;
            }
        }
    }
}