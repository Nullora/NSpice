// fuck windows this was hell to code 
// setup alone took me legit an hour thats on god
#include"spice.hpp"
#include <windows.h>
#include <shellapi.h>
#include<iostream>
#include<thread>
#include<chrono>
using namespace std;

NOTIFYICONDATA nid = {};
HWND hwnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_APP + 1) {
        if (lp == WM_RBUTTONUP) {
            // show context menu here
            HMENU menu = CreatePopupMenu();
            AppendMenu(menu, MF_STRING, 1, "Quit");
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            int cmd = TrackPopupMenu(menu, TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
            if (cmd == 1) PostQuitMessage(0);
            DestroyMenu(menu);
        }
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wp, lp);
}

void setupTray() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "NSpiceTray";
    RegisterClass(&wc);

    hwnd = CreateWindow("NSpiceTray", "", 0, 0, 0, 0, 0,
        HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);

    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_APP + 1;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    strcpy_s(nid.szTip, "NSpice");
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void removeTray() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void runMessageLoop() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void addToStartup() {
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    RegSetValueEx(hKey, "NSpice", 0, REG_SZ, (BYTE*)path, strlen(path) + 1);
    RegCloseKey(hKey);
}

string latest_ver;
string ver;
string apply_out;
string spotify_ver;
string last_spotify_ver;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    addToStartup();
    setupTray();
    thread checker([](){
        while(true){
            latest_ver = getVersionNumber();
            ver = exec("spicetify --version");
            ver.pop_back();
            //apply problem
            apply_out = exec("spicetify apply");
            if(apply_out.find("mismatched") != string::npos){
                exec("spicetify backup apply");
            }
            //auto update
            if(ver!=latest_ver){
                cout<<exec("spicetify update")<<endl;
                system("spicetify backup apply");
                system("taskkill /F /IM Spotify.exe & start \"\" \"%APPDATA%\\Spotify\\Spotify.exe\"");
                nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
                strcpy_s(nid.szInfoTitle, "NSpice");
                strcpy_s(nid.szInfo, "Spicetify Updated!");
                Shell_NotifyIcon(NIM_MODIFY, &nid);
            }
            //once every 2 hours
            this_thread::sleep_for(chrono::hours(2));
        }
    });
    checker.detach();
    runMessageLoop();
    removeTray();
    return 0;
}