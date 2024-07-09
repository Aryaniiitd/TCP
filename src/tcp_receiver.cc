#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();

    if (!head.syn && !_synReceived) {
        return;
    }

    // extract data from the payload
    string data = seg.payload().copy();

    bool endfile = false;
    bool t=true;

    // first SYN received
    if ( _synReceived==false && head.syn ==t) {
    _synReceived = true;
    	 _isn = head.seqno;
        // handles if the FIN and SYN are handled simultaneously 
        if(head.fin){
           _finReceived=true;
           endfile=true;
           }
       
        _reassembler.push_substring(data, 0, endfile);
        return;
    }

    // FIN received
    if (_synReceived==t&& head.fin) {
        _finReceived = endfile = true;
    }

    // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;

    // push the data into stream reassembler
    _reassembler.push_substring(data, stream_idx, endfile);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (_synReceived==false) {
        return nullopt;
    }
    return wrap(_reassembler.ack_index() + 1 + (_reassembler.empty() && _finReceived), _isn);
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
