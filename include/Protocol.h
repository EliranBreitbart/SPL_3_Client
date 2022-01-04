//
// Created by elira on 04/01/2022.
//

#ifndef BOOST_ECHO_CLIENT_PROTOCOL_H
#define BOOST_ECHO_CLIENT_PROTOCOL_H


class Protocol {
public:
    void process(T message);
    boolean shouldTerminate();

private:
    boolean terminate = false;
};


#endif //BOOST_ECHO_CLIENT_PROTOCOL_H
