#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace SerialUtils {

static const int MAX_SUPPORTED_MTRS = 3;

static const char DELIMITER = '\n';

enum CmdType {
    CMDTYPE_MTRS = 0,          // Actuate motors
    CMDTYPE_CAL,               // Calibrate motors
    CMDTYPE_ENDEFF_ON,         // Turn ee on
    CMDTYPE_ENDEFF_OFF,        // Turn ee off
    CMDTYPE_RESP
};

struct CmdMsg {
    /* Type of message */
    CmdType cmd_type;

    /* Data to send to Teensy */
    bool is_relative;
    float mtr_angles[3];
    
    /* Data to be received from Teensy */
    bool cmd_success;

    /* operators */
    operator std::string() const
    {
        std::ostringstream oss;
        oss << "CmdMsg " << this << ":" << std::endl
            << "    cmd_type: " << this->cmd_type << std::endl
            << "    is_relative: " << this->is_relative << std::endl
            << "    m1_angle: " << this->mtr_angles[0] << std::endl
            << "    m2_angle: " << this->mtr_angles[1] << std::endl
            << "    m3_angle: " << this->mtr_angles[2] << std::endl
            << "    cmd_success: " << this->cmd_success << std::endl;
        return oss.str();
    }
} __attribute__((packed));

union Packet {
    CmdMsg msg;
    char bytes[sizeof(CmdMsg)];
};

inline void pack(std::vector<char> &dest, CmdMsg &src) {
    Packet pkt = {.msg = src};
    dest.insert(dest.end(), pkt.bytes, pkt.bytes + sizeof(CmdMsg));
    dest.push_back(DELIMITER);
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
