#ifndef BUILD_INFO_H
#define BUILD_INFO_H

// ����ʱ��
#define BUILD_TIMESTAMP 20240219
constexpr const int build_timestamp = BUILD_TIMESTAMP;
// �����汾
#define BUILD_VERSION "v2024.02.19"
constexpr const char* build_version = BUILD_VERSION;
// �����汾��־
#define BUILD_VERSION_PARSE_TAG "build_version_parse_tag:{{v2024.02.19}}:build_version_parse_tag"
constexpr const char* build_version_parse_tag = BUILD_VERSION_PARSE_TAG;



// �����汾
#define GIT_VERSION ""
// ������ϣ
#define GIT_HASH "96dbbb6"
// ������֧
#define GIT_BRANCH "dev"

#endif // BUILD_INFO_H
