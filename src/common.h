#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>

#include <chrono>

const unsigned int WIN_W = 640; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 480;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

std::string LoadTextFile(const std::string &filename);
const char* loadFile( const char* filename ) ;

#endif