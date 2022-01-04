//
// Created by eliranbr@wincs.cs.bgu.ac.il on 04/01/2022.
//

#include "../include/socketReader.h"

socketReader::socketReader(ConnectionHandler* _connectionHandler) : connectionHandler(_connectionHandler){}

void socketReader::run() {
    while (1) {
        std::string answer;
        if (!connectionHandler->getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        int len = answer.length();
        answer.resize(len - 1); //TODO check
        std::cout << answer;
        if (answer == "bye") { //TODO change
            std::cout << "Exiting...\n" << std::endl;
            break;
        }
    }
}