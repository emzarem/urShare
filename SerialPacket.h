#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace SerialUtils {

struct CmdMsg {
    /* Data to send to Teensy */
    bool is_relative;
    float m1_angle, m2_angle, m3_angle;
    /* Data to be received from Teensy */
    bool motors_done;

    operator std::string() const
    {
        std::ostringstream oss;
        oss << "CmdMsg " << this << ":" << std::endl
            << "    is_relative: " << this->is_relative << std::endl
            << "    m1_angle: " << this->m1_angle << std::endl
            << "    m2_angle: " << this->m2_angle << std::endl
            << "    m3_angle: " << this->m3_angle << std::endl
            << "    motors_done: " << this->motors_done << std::endl;
        return oss.str();
    }
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
