#pragma once
#include <string>

std::string exec(const char* cmd);
std::string getVersionNumber();
//windows only function
std::string getSpotifyVersion();

//linux only function
std::string getSpotifyVersionLIN();
//both linux and windows now (i think)
void addToStartup();