#include <string>
#include <iostream>
#include <unistd.h>
#include <gtk/gtk.h>

#include "desktop_loading.h"
#include "search.h"

GtkBuilder *builder;
GtkWindow *window;
GtkEntry *search_entry;
GtkLabel *result_labels[5];
GtkImage *result_images[5];


void on_search_changed(GObject* object) {
    std::string query = std::string(gtk_entry_get_text(search_entry));
    search(query, all_desktops);
    for (int i = 0; i < 5; i++) {
        if (i >= result.size()) {
            // If there isn't a result to put here
            gtk_widget_hide(GTK_WIDGET(result_labels[i]));
            gtk_widget_hide(GTK_WIDGET(result_images[i]));
        } else {
            Desktop* entry = result.top();
            result.pop();

            // If there IS a result to put here
            gtk_widget_show(GTK_WIDGET(result_labels[i]));
            gtk_widget_show(GTK_WIDGET(result_images[i]));

            gtk_label_set_text(result_labels[i], entry->name.c_str());
            //TODO: Set Icon
        }
    }
}

void init_gui() {
    builder = gtk_builder_new_from_file("view.glade");

    window = GTK_WINDOW(gtk_builder_get_object(builder, "main"));
    gtk_window_set_decorated(window, FALSE);
    g_signal_connect(G_OBJECT(window), "focus-out-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    search_entry = GTK_ENTRY(gtk_builder_get_object(builder, "input"));
    g_signal_connect(G_OBJECT(search_entry), "changed", G_CALLBACK(on_search_changed), NULL);

    for (int i = 1; i < 6; i++) {
        std::string label_name = "result-text-" + std::to_string(i);
        std::string image_name = "result-image-" + std::to_string(i);
        result_labels[i-1] = GTK_LABEL(gtk_builder_get_object(builder, label_name.c_str()));
        result_images[i-1] = GTK_IMAGE(gtk_builder_get_object(builder, image_name.c_str()));
    }
}

int main(int argc, char** argv) {
    /**
     * Entrypoint
     */
    
    get_all_desktops();

    gtk_init(&argc, &argv);
    init_gui();
    
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    return 0;
}

/*
 * auto search_res = std::priority_queue<Desktop*, std::vector<Desktop*>, CompareDesktops>();
    search("intellij", all_desktops);
 */

