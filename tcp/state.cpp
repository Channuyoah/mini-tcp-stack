/*
  实现状态机逻辑
  TCP的协议规范 参考RFC793状态转换规则
*/

#include "state.h"

// 根据当前的状态和收到的报文决定状态是否切换，是否发送响应报文
// void tcp_process(tcp_t *tcp, tcp_packet_t *pkt);

const char* state_to_string(tcp_state_t state)
{
    switch(state) {
      case CLOSED: return "CLOSED";
      case LISTEN: return "LISTEN";
      case SYN_SENT: return "SYN_SENT";
      case SYN_RECEIVED: return "SYN_RECEIVED";
      case ESTABLISTED: return "ESTABLISTED";
      case FIN_WAIT_1: return "FIN_WAIT_1";
      case FIN_WAIT_2: return "FIN_WAIT_2";
      case CLOSE_WAIT: return "CLOSE_WAIT";
      case LAST_ACK: return "LAST_ACK";
      case TIME_WAIT: return "TIME_WAIT";
      default: return "UNKNOWN";
    }
}
