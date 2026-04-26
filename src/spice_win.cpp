// fuck windows this was hell to code 
// setup alone took me legit an hour thats on god
#include"spice.hpp"
#include <windows.h>
#include <shellapi.h>
#include<thread>
#include<chrono>
using namespace std;

NOTIFYICONDATA nid = {};
HWND hwnd;
//the whole system tray in the toolbar is fully ai btw the actual logic is what i coded
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

string latest_ver;
string ver;
string apply_out;
int timet = 120;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    addToStartup();
    setupTray();
    //first notif 
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
    strcpy_s(nid.szInfoTitle, "NSpice");
    strcpy_s(nid.szInfo, "Running first check.");
    Shell_NotifyIcon(NIM_MODIFY, &nid);
    exec("spicetify update");
    exec("spicetify apply");
    //loop (every 2 hours)
    thread checker([](){
        while(true){
            timet = 60;
            latest_ver = getVersionNumber();
            //  panic mode (no wifi/rate limited/ corrupted JSON)
            if (latest_ver=="ERROR: 6767") {
                //send warning to our cute little user ..... 
                nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
                strcpy_s(nid.szInfoTitle, "NSpice");
                strcpy_s(nid.szInfo, "No WiFi. Switching to panic mode.");
                Shell_NotifyIcon(NIM_MODIFY, &nid);
                
                //  check every 30 mins to wait for wifi to come back and recheck
                timet = 30;
                this_thread::sleep_for(chrono::minutes(timet));
                continue;
            }

            ver = exec("spicetify --version");
            ver.pop_back();

            //auto update
            if(ver!=latest_ver){
                nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
                strcpy_s(nid.szInfoTitle, "NSpice");
                string msg = "Spicetify Out-Of-Date. Current: " + ver + " Latest: " + latest_ver;
                strcpy_s(nid.szInfo, msg.c_str());
                Shell_NotifyIcon(NIM_MODIFY, &nid);
                exec("spicetify update");
                apply_out = exec("spicetify apply");
                if(apply_out.find("mismatched") != string::npos){
                    exec("spicetify backup apply");
                }
                nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
                strcpy_s(nid.szInfoTitle, "NSpice");
                strcpy_s(nid.szInfo, "Spicetify Updated.");
                Shell_NotifyIcon(NIM_MODIFY, &nid);
            }
            //in minutes so i can handle panic mode later
            this_thread::sleep_for(chrono::minutes(timet));
        }
    });
    checker.detach();
    runMessageLoop();
    removeTray();
    return 0;
}