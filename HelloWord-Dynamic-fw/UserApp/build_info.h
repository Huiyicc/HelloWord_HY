#ifndef BUILD_INFO_H
#define BUILD_INFO_H

// ����ʱ��
#define BUILD_TIMESTAMP 20240126
constexpr const int build_timestamp = BUILD_TIMESTAMP;
// �����汾
#define BUILD_VERSION "v2024.01.26"
constexpr const char* build_version = BUILD_VERSION;
// �����汾��־
#define BUILD_VERSION_PARSE_TAG "build_version_parse_tag:{{v2024.01.26}}:build_version_parse_tag"
constexpr const char* build_version_parse_tag = BUILD_VERSION_PARSE_TAG;



// �����汾
#define GIT_VERSION ""
// ������ϣ
#define GIT_HASH "c7c095c"
// ������֧
#define GIT_BRANCH "dev"

#endif // BUILD_INFO_H
