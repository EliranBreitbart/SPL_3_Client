#include "../include/connectionHandler.h"

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port): host_(host), port_(port), io_service_(), socket_(io_service_){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line, ';');
}

bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line, ';');
}
 
bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    try {
		do{
			if(!getBytes(&ch, 1))
                return false;
            frame.append(1, ch);
        }while (delimiter != ch);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    decode(frame);
    return true;
}
 
bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
    std::vector<char> encoded = encode(const_cast<string &>(frame));
    char* a = &encoded[0];
	bool result=sendBytes(a,encoded.size());
	if(!result) return false;
	return sendBytes(&delimiter,1);
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

short ConnectionHandler::bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

void ConnectionHandler::shortToBytes(short num, char* bytesArr)
{
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

void ConnectionHandler::decode(std::string &frame){
    std::string toPrint = "";
    string allToPrint = "";
    char opcode[2];
    opcode[0] = frame[0];
    opcode[1] = frame[1];
    short op = bytesToShort(opcode);
    frame = frame.substr(2);
    switch (op) {
        case 9: //notification
            toPrint += "NOTIFICATION- ";
            if(frame[0] == '0')
                toPrint += "PM ";
            else{
                if(frame[0] == '1')
                    toPrint += "Public ";
            }
            frame = frame.substr(1);
            while (frame[0] != '\0') { //user that sent the msg
                toPrint += frame[0];
                frame = frame.substr(1);
            }
            frame = frame.substr(1);
            toPrint += ": ";
            while (frame[0] != '\0') { //msg content
                toPrint += frame[0];
                frame = frame.substr(1);
            }
//            while(frame.length()>0){
//                if (frame[0] == '\0'){
//                    if(frame.length() != 1)
//                        toPrint += " ";
//                }
//                else{
//                    toPrint += frame[0];
//                }
//                frame = frame.substr(1);
//            }
            //cout << toPrint;
            break;
        case 10: { //ACK
            toPrint += "ACK ";
            char senderOp[2];
            senderOp[0] = frame[0];
            senderOp[1] = frame[1];
            short senderOpcode = bytesToShort(senderOp);
            toPrint += std::to_string(senderOpcode);
            frame = frame.substr(2);
            frame.pop_back();
            switch (senderOpcode) {
                //case 1: //register
                //case 2: //login
                //case 3: //logout
                //case 5: //post
                //case 6: //pm
                //case 12: //block
                    // cout << toPrint <<std::endl;
                    //break;
                case 4: //follow/unfollow
                    toPrint += " ";
                    while (frame[0] != '\0') { //username
                        toPrint += frame[0];
                        frame = frame.substr(1);
                    }
                    //cout << toPrint <<std::endl;
                    break;
                case 7: //logstat
                case 8: //stat
                    while (frame.length() > 0) {
                        toPrint += " ";
                        char age[2];
                        age[0] = frame[0];
                        age[1] = frame[1];
                        short shortAge = bytesToShort(age);
                        toPrint += std::to_string(shortAge);
                        frame = frame.substr(2);
                        toPrint += " ";

                        char numPosts[2];
                        numPosts[0] = frame[0];
                        numPosts[1] = frame[1];
                        short shortNumPosts = bytesToShort(numPosts);
                        toPrint += std::to_string(shortNumPosts);
                        frame = frame.substr(2);
                        toPrint += " ";

                        char numFollowers[2];
                        numFollowers[0] = frame[0];
                        numFollowers[1] = frame[1];
                        short shortNumFollowers = bytesToShort(numFollowers);
                        toPrint += std::to_string(shortNumFollowers);
                        frame = frame.substr(2);
                        toPrint += " ";

                        char numFollowing[2];
                        numFollowing[0] = frame[0];
                        numFollowing[1] = frame[1];
                        short shortNumFollowing = bytesToShort(numFollowing);
                        toPrint += std::to_string(shortNumFollowing);
                        frame = frame.substr(2);
                        if(frame.size() > 4) {
                            frame = frame.substr(5);//remove divider
                        }else if(frame.size() == 1){
                            frame = frame.substr(1);
                            }
                        allToPrint += toPrint;
                        //cout << toPrint <<std::endl;
                        toPrint.clear();
                        toPrint = "\nACK " + std::to_string(senderOpcode);
                    }
                    toPrint.clear();
                    toPrint = allToPrint;
                    allToPrint.clear();
                    break;
            }
            break;
        }
        case 11: //error
            toPrint += "ERROR ";
            char sendOp[2];
            sendOp[0] = frame[0];
            sendOp[1] = frame[1];
            short shortSendOp = bytesToShort(sendOp);
            toPrint += std::to_string(shortSendOp);
            //cout << toPrint <<std::endl;
        break;
    }
    frame.clear();
    frame = toPrint;
    toPrint.clear();
}

std::vector<char> ConnectionHandler::encode(std::string &msg) {
    std::vector<char> result;
    std::istringstream iss(msg);
    std::string word;
    char op[2];
    iss >> word;
    if (word == "REGISTER") {
        shortToBytes((short) 1, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        for(int i = 0; i < 3; i++){
            iss >> word;
            for (char &k: word) {
                result.push_back(k);
            }
            result.push_back('\0');
        }
    } else if(word =="LOGIN"){
        shortToBytes((short) 2, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        for(int i = 0; i < 2; i++) {
            iss >> word;
            for (char &k: word) {
                result.push_back(k);
            }
            result.push_back('\0');
        }
        iss >> word;
        if(word =="1"){
            result.push_back('\1');
        }else{
            result.push_back('\0');
        }
    } else if(word == "LOGOUT"){
        shortToBytes((short) 3, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
    } else if(word == "FOLLOW"){
        shortToBytes((short) 4, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        iss>>word;
        if(word == "0") {
            result.push_back('\0');
        } else
            result.push_back('\1');
        iss >> word;
        for (char &k: word) {
            result.push_back(k);
        }
        result.push_back('\0');
    } else if(word =="POST"){
        shortToBytes((short) 5, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        for(unsigned int i = word.length() + 1; i < msg.length(); i++){
            result.push_back(msg[i]);
        }
        result.push_back('\0');
    } else if(word =="PM"){
        shortToBytes((short) 6, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        iss >> word;
        int index = word.length();
        for (char &k: word) {
            result.push_back(k);
        }
        result.push_back('\0');
        for(unsigned int i = 4   + index; i < msg.length(); i++){
            result.push_back(msg[i]);
        }
        result.push_back('\0');
//        std::time_t t = std::time(0);   // get time now
//        std::tm* now = std::localtime(&t);
//        std::string str = std::to_string(now->tm_mday) + "-" + std::to_string(now->tm_mon + 1) + '-' + std::to_string(now->tm_year + 1900);
//        for (char &k: str) {
//            result.push_back(k);
//        }
//        result.push_back('\0');
    } else if(word =="LOGSTAT"){
        shortToBytes((short) 7, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
    } else if(word =="STAT"){
        shortToBytes((short) 8, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        for( unsigned int i = 2; i < msg.length(); i++){
            result.push_back(msg[i]);
        }
        result.push_back('\0');
    } else if(word =="BLOCK"){
        shortToBytes((short) 12, op);
        result.push_back(op[0]);
        result.push_back(op[1]);
        iss >> word;
        for(unsigned int i = 0; i < word.length(); i++){
            result.push_back(word[i]);
        }
        result.push_back('\0');
    }
    return result;
}

bool ConnectionHandler::isTerminated() const {
    return terminated;
}

void ConnectionHandler::setTerminated(bool terminated) {
    ConnectionHandler::terminated = terminated;
}
