#include "Buffer.h"


Buffer::Buffer()
{

}

Buffer::~Buffer()
{

}

const char* Buffer::peek()
{
    return buf_.c_str();
}

int Buffer::readableBytes()
{
    return buf_.size();
}

void Buffer::retrieve(int len)
{
    buf_ = buf_.substr(len, buf_.size());
}

void Buffer::append(const std::string& buf)
{
    buf_.append(buf);
}

std::string Buffer::retrieveAsString(size_t len)
{
    std::string result(peek(), len);
    retrieve(len);
    return result;
}

std::string Buffer::retrieveAllAsString()
{
    return retrieveAsString(buf_.size());
}

