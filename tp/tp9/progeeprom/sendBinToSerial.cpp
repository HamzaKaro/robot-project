#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

const int MAX_BYTE_PER_SEND = 8;
const int MS_TO_SLEEP = 50;

int main(int argc, const char** argv)
{
    if (argc != 3)
    {
        std::cout << "Invalid arguments. Ex: > sendBinToSimulide /dev/pts/5 myProg.bin" << std::endl;
        return -1;
    }

    std::fstream binFile(argv[2], std::fstream::in);
    
    if (binFile.fail())
    {
        std::cout << "The file \""<< argv[2] << "\" cannot be opened in read mode." << std::endl;
        return -1;
    }
    
    std::fstream serialPort(argv[1], std::fstream::out);
    
    if (serialPort.fail())
    {
        std::cout << "The file \""<< argv[1] << "\" cannot be opened in write mode." << std::endl;
        return -1;
    }
    
    int byteCounter = 0;
    std::string data = "";
    
    while (binFile.peek() != std::char_traits<char>::eof())
    {
        char c = binFile.get();
        
        data += c;
        byteCounter++;
        if (byteCounter >= MAX_BYTE_PER_SEND)
        {
            serialPort.write(data.c_str(), byteCounter);
            serialPort.flush();
            data = "";
            byteCounter = 0;
            usleep(MS_TO_SLEEP*1000);
        }
    }
    
    if (byteCounter < MAX_BYTE_PER_SEND)
    {
        serialPort.write(data.c_str(), byteCounter);
        serialPort.flush();
    }
}
