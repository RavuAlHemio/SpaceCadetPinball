#include "imgui_draw.cpp"


char* DecompressCompressedBase85Data(const char* compressed_data_base85)
{
	int compressed_data_size = (((int)strlen(compressed_data_base85) + 4) / 5) * 4;
	void* compressed_data = IM_ALLOC((size_t)compressed_data_size);
	Decode85((const unsigned char*)compressed_data_base85, (unsigned char*)compressed_data);

	const unsigned int buf_decompressed_size = stb_decompress_length((const unsigned char*)compressed_data);
	unsigned char* buf_decompressed_data = (unsigned char*)IM_ALLOC(buf_decompressed_size);
	stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_data, (unsigned int)compressed_data_size);
	IM_FREE(compressed_data);
	return (char*)buf_decompressed_data;
}

char* DecompressCompressedStbData(const unsigned int* compressed_data_stb, unsigned int compressed_data_size, unsigned int& decompressed_size)
{
	decompressed_size = stb_decompress_length((const unsigned char*)compressed_data_stb);
	unsigned char* buf_decompressed_data = (unsigned char*)IM_ALLOC(decompressed_size);
	stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_data_stb, (unsigned int)compressed_data_size);
	return (char*)buf_decompressed_data;
}
