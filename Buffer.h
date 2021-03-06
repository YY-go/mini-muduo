#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer
{
public:
    Buffer();
    ~Buffer();
    const char* peek();
    int readableBytes();
    void retrieve(int len);
    void append(const std::string& buf);
    std::string retrieveAllAsString();
    std::string retrieveAsString(size_t len);
private:
    std::string buf_;
};

#endif
