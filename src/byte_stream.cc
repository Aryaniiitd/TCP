#include "byte_stream.hh"

#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t c)

: buffer(), capacity(c), endwrite(false), endread(false), writebytes(0), readbytes(0) {}



size_t ByteStream::write(const string &data) {
  size_t cWrite = capacity - buffer.size();
    size_t reWrite = min(cWrite, data.length());
    size_t a=0;
    while(a<reWrite) {
        buffer.push_back(data[a]);
        a++;
    }
    writebytes += reWrite;
    return reWrite;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
size_t Peek = min(len, buffer.size());
size_t a=0;
    string out = "";
    
    while(a<Peek) {
        out += buffer[a];
        a++;
    }
    return out;
  
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
size_t a=0;
if (len > buffer.size()) {
        set_error();
        return;
    }
   while(a<len){
        buffer.pop_front();
        a++;
    }
    readbytes += len;

}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
		string out = "";
		size_t a=0;
    if (len > buffer.size()) {
        set_error();
        return out;
    }
    for (a = 0; a < len; a++) {
        out += buffer.front();
        buffer.pop_front();
    }
    readbytes += len;
  return out;
}
bool ByteStream::buffer_empty() const {return buffer.empty(); }
size_t ByteStream::remaining_capacity() const { return capacity - buffer.size(); }

void ByteStream::end_input() { endwrite=true;}

bool ByteStream::input_ended() const { return endwrite;}

size_t ByteStream::bytes_read() const { return readbytes;  }
size_t ByteStream::buffer_size() const {return buffer.size(); }

bool ByteStream::eof() const { return buffer.empty()&&endwrite; }

size_t ByteStream::bytes_written() const { return writebytes; }



