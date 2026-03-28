#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <thread>
#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>
using namespace std;

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
int main(int argc, char* argv[]){
    string version;
    gtk_init(&argc, &argv);
    AppIndicator* indicator = app_indicator_new("spice-updater", "system-software-update", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    GtkWidget* menu = gtk_menu_new();
    GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(quit_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit_item);
    gtk_widget_show_all(menu);
    app_indicator_set_menu(indicator, GTK_MENU(menu));
    thread checker([](){
        string version;
        while(true){
            version = exec("spicetify --version");
            version.pop_back();
            if(version != getVersionNumber()){
                cout << "updating spicetify...\n";
                system("spicetify update");
            }
            this_thread::sleep_for(chrono::minutes(30));
        }
    });
    checker.detach();
    gtk_main();
}