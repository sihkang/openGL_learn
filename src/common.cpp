#include "common.h"

std::string LoadTextFile(const std::string &filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open())
	{
		return {}; // optional file의 아무것도 없는 경우를 {}로 표기
	}
	std::stringstream text;
	text << fin.rdbuf();
	return text.str();
}


const char* loadFile( const char* filename ) {
	FILE* fp = fopen( filename, "r" );
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open \"%s\"\n", filename);
		return NULL;
	}
	// get file size to allocate a buffer
	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	rewind(fp);
	char* buf = (char*)malloc(sizeof(char) * (len + 4));
	// read in the whole contents: (ASSUMPTION: small file size)
	size_t size = fread(buf, sizeof(char), len, fp);
	fclose(fp);
	buf[size] = '\0';
	// done
	return (const char*)buf;
}
