#ifndef HEADER_COMMON_SENDWND
#define HEADER_COMMON_SENDWND

#include <queue>
#include <mutex>
#include <unordered_map>

#include "ISendWnd.h"
#include "CommonType.h"

namespace hudp {

    class CPriorityQueue;
    class CIncrementalId;
    class CMsg;
    // send window
    class CSendWndImpl : public CSendWnd {
    public:
        CSendWndImpl(uint16_t send_wnd_size, CPriorityQueue* priority_queue, bool always_send = false);
        ~CSendWndImpl();

        void PushBack(CMsg* msg);
        // receive a ack
        void AcceptAck(uint16_t id);
        void AcceptAck(uint16_t start_id, uint16_t len);
        void AcceptAck(std::vector<uint16_t>& vec_id, uint16_t start_index, uint16_t len);

        // change send window size
        void ChangeSendWndSize(uint16_t size);

        // remove all msg
        void Clear();

    private:
        // call callback func
        void SendAndAck();
        // send next
        void SendNext();
        // push back to send wnd
        void PushBackToSendWnd(CMsg* msg);
        // add to list end
        void AddToEnd(CMsg* msg);
        // remove from list
        void Remove(CMsg* msg);

    private:
        // point to the send list start
        // all msg will be added here after send
        CMsg*           _start;
        CMsg*           _end;
        // when ack is not start, increase id
        // when _out_of_order_count >= 3, resend _start msg quickly
        uint16_t        _out_of_order_count;
        
        std::mutex      _mutex;
        uint16_t        _send_wnd_size;
        uint16_t        _cur_send_size;
        // only orderly use
        bool            _always_send;

        CPriorityQueue* _priority_queue;
        CIncrementalId* _incremental_id;

        std::queue<CMsg*>                   _ack_queue;
        std::queue<CMsg*>                   _send_queue;
        std::unordered_map<uint16_t, CMsg*> _id_msg_map;
    }; 
    
}
#endif