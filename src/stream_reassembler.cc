#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`


using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : first_index(0)
    , reassemble(0)
    , _capacity(capacity)
    , _output(capacity)
    , _eof(0)
    , buffer(capacity, '\0')
    , buf_bitmap(capacity, false)
{}

//! \details This functions calls just after pushing a substring into the
//! _output stream. It aims to check if there exists any contiguous substrings
//! recorded earlier can be push into the stream.
void StreamReassembler::check(){
string tmp = "";
    size_t count=0;
    while (buf_bitmap.front()) {
        tmp += buffer.front();
        buffer.pop_front();
        buf_bitmap.pop_front();
        buffer.push_back('\0');
        buf_bitmap.push_back(false);
        first_index ++;
    }
  
    if (!tmp.empty()) {

        reassemble -= tmp.length();
        _output.write(tmp);
        
    }
}
void StreamReassembler::check_contiguous() {
    
    check();
}

size_t StreamReassembler::unassembled_bytes() const { return reassemble; }

void StreamReassembler::push_substring(const string &data,const size_t index, const size_t eof) {
 // ignore invalid index
    if (index > first_index + _capacity) return;
    
    size_t size = data.length();
    
    if (size == 0 && _eof && reassemble == 0) {
        _output.end_input();
        return;
    }

    if (eof) {
        _eof = 1;
    }
    // handle out of order data
    if (index >= first_index) {
        int null = index - first_index;
        size_t small=_capacity - _output.buffer_size() - null;
        size_t normal = min(size, small);
        if (normal < size) {
            _eof = false;
        }
        size_t i=0;
 for (i; i < normal; ++i) {
        size_t ll=i+null;
            if (!buf_bitmap[ll]){
             reassemble++;
             size_t l=data[i]; 
            buffer[ll] = l;
            buf_bitmap[ll] = true;
           }
        }
    } 
    // handle repeation of data
    else if (index + size > first_index) {
    size_t cc=_capacity - _output.buffer_size();
        size_t normal = min(index+ size - first_index , cc);
        size_t k=index+ size - first_index ;
        if (k>normal ) {
            _eof = false;
        }
        size_t i =0;
        for(i;i<normal;++i) {
             size_t ll=i+first_index - index;
            if (!buf_bitmap[i]){
            buffer[i] = data[ll];
            buf_bitmap[i] = true;
            reassemble++;
            }
            
        }
    }
     if (size == 0 && _eof ) {
     if(reassemble == 0){
        _output.end_input();
        return;}
    }
    check_contiguous();
    if (_eof && reassemble == 0) {
        _output.end_input();
    }
}




bool StreamReassembler::empty() const { return reassemble == 0; }

size_t StreamReassembler::ack_index() const { return first_index; }

