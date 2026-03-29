#include"spice.hpp"
#include<iostream>
#include<thread>
#include<chrono>
#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>
using namespace std;
int main(int argc, char* argv[]){
    string version;
    gtk_init(&argc, &argv);

    AppIndicator* indicator = app_indicator_new("Novus-Spice", "system-software-update", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    GtkWidget* menu = gtk_menu_new();
    GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(quit_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit_item);
    gtk_widget_show_all(menu);
    app_indicator_set_menu(indicator, GTK_MENU(menu));
    system("notify-send 'NSpice' 'Running first check...'");
    thread checker([](){
        string version;
        while(true){
            version = exec("spicetify --version");
            version.pop_back();
            if(version != getVersionNumber()){
                system("spicetify update");
                system("notify-send 'NSpice' 'Updated spicetify!'");
                string in = exec("spicetify apply");
                if(in.find("mismatch")!= string::npos){
                    exec("spicetify backup apply");
                }
            }
            this_thread::sleep_for(chrono::hours(1));
        }
    });
    checker.detach();
    gtk_main();
}