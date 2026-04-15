#include "spice.hpp"
#include <array>
#include <memory>
#include <stdexcept>
//include windows if on windows ! >:)
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

string getSpotifyVersion(){ // windows only
    return exec("powershell -command \"(Get-Item '$env:APPDATA\\Spotify\\Spotify.exe').VersionInfo.FileVersion\"");
}

string getSpotifyVersionLIN() {
    //i think you need to install with snap not sure
    return exec("spotify --version 2>/dev/null | awk '{print $3}'");
}

string exec(const char* cmd) {
    string result;
#ifdef _WIN32
    // works in background for windows so it dosent flash 5 terminals
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return "";
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = {sizeof(STARTUPINFOA)};
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi = {0};
    if (CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hWrite);
        char buffer[128];
        DWORD bytesRead;
        while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            result += buffer;
        }
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    CloseHandle(hRead);
#else
    // old dandy exec for linux
    array<char, 128> buffer;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw runtime_error("popen() failed");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
#endif
    return result;
}
string getVersionNumber(){
    string json = exec("curl -s -L -A \"spicerupdater\" https://api.github.com/repos/spicetify/spicetify-cli/releases/latest");
    // detect error to not spam updates if wifi is down
    if (json.rfind("curl:", 0) == 0) {
        return "ERROR: 6767";
    }

    //find tag
    int pos = json.find("\"tag_name\":\"v");
    if (pos==string::npos) return "ERROR: 6767"; // if no tag was found
    //actual tag shit
    int start = pos+13;
    int end = json.find("\"", start);
    string rest = json.substr(start, end-start);
    return rest;
}

void addToStartup() {
#ifdef _WIN32
    //windows
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    RegSetValueEx(hKey, "NSpice", 0, REG_SZ, (BYTE*)path, strlen(path) + 1);
    RegCloseKey(hKey);
#else
    // linux
    string home = getenv("HOME");
    string path = home + "/.config/autostart/nspice.desktop";
    FILE* f = fopen(path.c_str(), "w");
    if(f) {
        fprintf(f, "[Desktop Entry]\nType=Application\nName=NSpice\nExec=/usr/local/bin/nspice\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\n");
        fclose(f);
    }
#endif
}