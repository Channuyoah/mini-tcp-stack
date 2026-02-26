/*
  定义状态枚举
  tcp是一个有限状态自动机,这个枚举是TCP的完整生命周期
*/

#ifndef STATE_H
#define STATE_H

typedef enum {
    CLOSED,
    LISTEN,
    SYN_SENT,
    SYN_RECEIVED,
    ESTABLISTED,
    FIN_WAIT_1,
    FIN_WAIT_2,
    CLOSE_WAIT,
    LAST_ACK,
    TIME_WAIT,
} tcp_state_t;

// const char* state_to_string(tcp_state_t, state);

#endif
