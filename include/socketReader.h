//
// Created by eliranbr@wincs.cs.bgu.ac.il on 04/01/2022.
//

#ifndef HW3_CLIENT_SOCKETREADER_H
#define HW3_CLIENT_SOCKETREADER_H

#include <iostream>
#include <thread>
#include "../include/connectionHandler.h"
#include "keyboardReader.h"

class socketReader {
public:
    socketReader(ConnectionHandler* _connectionHandler, keyboardReader keyboard);
    void run();
private:
    ConnectionHandler *connectionHandler;
    keyboardReader keyboard;
};

#endif //HW3_CLIENT_SOCKETREADER_H
