#pragma once

#include <string>
#include <vector>

namespace SerialUtils {

struct CmdMsg {
    /* Data to send to Teensy */
    bool is_relative;
    float m1_angle, m2_angle, m3_angle;
    /* Data to be received from Teensy */
    bool motors_done;
};

union Packet {
    CmdMsg msg;
    char bytes[sizeof(CmdMsg)];
} __attribute__((packed));

inline void pack(std::vector<char> &dest, CmdMsg &src) {
    Packet pkt = {.msg = src};
    dest.insert(dest.end(), pkt.bytes, pkt.bytes + sizeof(CmdMsg));
}

inline void unpack(std::vector<char> &src, CmdMsg &dest) {
    Packet pkt;
    std::copy(src.begin(), src.end(), pkt.bytes);
    dest = pkt.msg;
}

inline void unpack(std::vector<char>::iterator start,
                   std::vector<char>::iterator end,
                   CmdMsg &dest) {
    Packet pkt;
    std::copy(start, end, pkt.bytes);
    dest = pkt.msg;
}


}  // namespace SerialUtils
