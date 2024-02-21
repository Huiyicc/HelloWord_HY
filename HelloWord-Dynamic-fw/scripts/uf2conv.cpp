//
// Created by 19254 on 2024/2/22.
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

auto UF2_MAGIC_START0 = 0x0A324655; // "UF2\n"
auto UF2_MAGIC_START1 = 0x9E5D5157; // Randomly selected
auto UF2_MAGIC_END = 0x0AB16F30; // Ditto

size_t BinToUf2(const std::string &BinFile, const std::string &Uf2File,uint32_t FamilyID =0x57755a57, uint32_t AppstartAddr = 0x8010000) {
  std::ifstream binFile(BinFile, std::ios::binary);
  auto file_content = std::string((std::istreambuf_iterator<char>(binFile)), std::istreambuf_iterator<char>());
  std::vector<uint8_t> datapadding;
  while (datapadding.size() < 512 - 256 - 32 - 4) {
    datapadding.push_back(0);
    datapadding.push_back(0);
    datapadding.push_back(0);
    datapadding.push_back(0);
  }
  auto num_blocks = static_cast<size_t>((file_content.size() + 255) / 256);
  std::vector<uint8_t> outp;
  for (size_t blockno = 0; blockno < num_blocks; blockno++) {
    auto ptr = 256 * blockno;
    std::vector<uint8_t> chunk(file_content.begin() +ptr, file_content.begin() + ptr + 256);
    auto flags = 0x0;
    flags |= 0x2000;
    std::vector<uint8_t> hd(32, 0);
    *(reinterpret_cast<uint32_t *>(&hd[0])) = UF2_MAGIC_START0;
    *(reinterpret_cast<uint32_t *>(&hd[0])) = UF2_MAGIC_START0;
    *(reinterpret_cast<uint32_t *>(&hd[4])) = UF2_MAGIC_START1;
    *(reinterpret_cast<uint32_t *>(&hd[8])) = flags;
    *(reinterpret_cast<uint32_t *>(&hd[12])) = ptr + AppstartAddr;
    *(reinterpret_cast<uint32_t *>(&hd[16])) = 256;
    *(reinterpret_cast<uint32_t *>(&hd[20])) = blockno;
    *(reinterpret_cast<uint32_t *>(&hd[24])) = num_blocks;
    *(reinterpret_cast<uint32_t *>(&hd[28])) = FamilyID;
    while (chunk.size() < 256) {
      chunk.push_back(0);
    }
    std::vector<uint8_t> block;
    block.insert(block.end(), hd.begin(), hd.end());
    block.insert(block.end(), chunk.begin(), chunk.end());
    block.insert(block.end(), datapadding.begin(), datapadding.end());
    std::vector<uint8_t> tmp(4,0);
    *(reinterpret_cast<uint32_t *>(&tmp[0])) = UF2_MAGIC_END;
    block.insert(block.end(), tmp.begin(), tmp.end());
    if (block.size() != 512) {
      throw std::runtime_error("block size error");
    }
    outp.insert(outp.end(), block.begin(), block.end());
  }
  std::ofstream uf2File(Uf2File, std::ios::binary);
  uf2File.write(reinterpret_cast<const char *>(outp.data()), outp.size());
  uf2File.close();
  binFile.close();
  return outp.size();
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <(input)bin file> <(out)uf2 file>" << std::endl;
    return 1;
  }
  std::string binFile = argv[1];
  std::string uf2File = argv[2];
  std::cout << "Converting " << binFile << " to " << uf2File << std::endl;
  std::cout << "Written "<< BinToUf2(binFile, uf2File) <<" bytes.";
  return 0;
}