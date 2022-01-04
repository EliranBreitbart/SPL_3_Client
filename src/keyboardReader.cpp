//
// Created by eliranbr@wincs.cs.bgu.ac.il on 04/01/2022.
//

#include "../include/keyboardReader.h"

keyboardReader::keyboardReader(ConnectionHandler* _connectionHandler) : connectionHandler(_connectionHandler){}

void keyboardReader::run() {
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len=line.length();
        if (!connectionHandler.sendLine(line)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
}