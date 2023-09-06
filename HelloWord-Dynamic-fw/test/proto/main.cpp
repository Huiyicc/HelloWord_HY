//
// Created by 19254 on 2023/9/6.
//

#include <cstdint>
#include <iostream>
#include "memory"
#include "git.hpp"
#include "pb_decode.h"
#include "pb_encode.h"
#include "hid_msg.pb.h"

bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    char *_str = (char*)*arg;
    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)_str, strlen(_str));
}

int main() {
    uint8_t buffer[65];
    memset(buffer, 0, sizeof(buffer));
    buffer[0]=0x04;
    buffer[1]=0;
    pb_ostream_t stream = pb_ostream_from_buffer(&buffer[3], 62);
    hid_msg_CtrlMessage msg = hid_msg_CtrlMessage_init_default;
    msg.id = hid_msg_MessageId_VERSION;
    hid_msg_Version version = hid_msg_Version_init_default;
    version.GitVer.funcs.encode = encode_string;
    version.GitVer.arg=(void*)GIT_VERSION;
    version.GitBranch.funcs.encode = encode_string;
    version.GitBranch.arg=(void*)GIT_BRANCH;
    version.GitHash.funcs.encode = encode_string;
    version.GitHash.arg=(void*)GIT_HASH;
    msg.which_payload= hid_msg_CtrlMessage_version_tag;
    msg.payload.version = version;
    auto status = pb_encode(&stream, hid_msg_CtrlMessage_fields, &msg);
    auto message_length = stream.bytes_written;
    if (message_length>62||!status) {
        // 数据错误
        buffer[2]=0;
        std::cout<<"error"<<std::endl;
        return 1;
    }
    buffer[2]=message_length;
    std::cout<<"success"<<std::endl;

    return 0;
}