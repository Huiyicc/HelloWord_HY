#ifndef BUILD_INFO_H
#define BUILD_INFO_H

// 构建时间
#define BUILD_TIMESTAMP 20240114
constexpr const int build_timestamp = BUILD_TIMESTAMP;
// 构建版本
#define BUILD_VERSION "v2024.01.14"
constexpr const char* build_version = BUILD_VERSION;
// 构建版本标志
#define BUILD_VERSION_PARSE_TAG "build_version_parse_tag:{{v2024.01.14}}:build_version_parse_tag"
constexpr const char* build_version_parse_tag = BUILD_VERSION_PARSE_TAG;



// 构建版本
#define GIT_VERSION ""
// 构建哈希
#define GIT_HASH "80c2b37"
// 构建分支
#define GIT_BRANCH "dev"

#endif // BUILD_INFO_H
