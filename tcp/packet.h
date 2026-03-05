/*
  定义报文结构 tcp建立连接和断开连接时的报文
  tips: 真实的报文格式应该是在RFC793当中
*/

#ifndef PACKET_H
#define PACKET_H

#pragma once
#include <string>
#include <cstdint>

class TCP;

struct Packet {
    // 控制位
    bool syn = false;
    bool ack = false;
    bool fin = false;
    
    // 序号字段
    uint32_t seq = 0;
    uint32_t ack_seq = 0;

    // 数据负载
    std::string payload;

    // 工具函数
    size_t length() const {
        return payload.size();
    }

    bool has_data() const {
        return !payload.empty();
    }
};

#endif
