#pragma once

char* DecompressCompressedBase85Data(const char* compressed_data_base85);
char* DecompressCompressedStbData(const unsigned int* compressed_data_stb, unsigned int compressed_data_size, unsigned int& decompressed_size);
