//
// Created by eliranbr@wincs.cs.bgu.ac.il on 04/01/2022.
//

#ifndef HW3_CLIENT_KEYBOARDREADER_H
#define HW3_CLIENT_KEYBOARDREADER_H
#include <iostream>
#include <thread>
#include "../include/connectionHandler.h"
class keyboardReader {
public:
    keyboardReader(ConnectionHandler* _connectionHandler);
    void run();
private:
    ConnectionHandler *connectionHandler;
};


#endif //HW3_CLIENT_KEYBOARDREADER_H
