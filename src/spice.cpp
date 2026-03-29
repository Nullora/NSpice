#include "spice.hpp"
#include <array>
#include <memory>
#include <stdexcept>
using namespace std;

string getSpotifyVersion(){
    return exec("powershell -command \"(Get-Item '$env:APPDATA\\Spotify\\Spotify.exe').VersionInfo.FileVersion\"");
}

string exec(const char* cmd){
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if(!pipe){
        throw runtime_error("popen() failed to run");
    }

    while(fgets(buffer.data(), buffer.size(), pipe.get())!=nullptr){
        result += buffer.data();
    }
    return result;
}
string getVersionNumber(){
    string json = exec("curl -s -L -A \"spice-updater\" https://api.github.com/repos/spicetify/spicetify-cli/releases/latest");
    int pos = json.find("\"tag_name\":\"v");
    int start = pos+13;
    int end = json.find("\"", start);
    string rest = json.substr(start, end-start);
    return rest;
}