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
Desktop *result_desktops[5];

int selected_desktop = 0;
int num_of_results = 0;

void cleanup() {
    clean_results();
    clean_entries();
}

void clean_exit() {
    cleanup();
    gtk_main_quit();
}

void update_selected() {
    if (num_of_results > 0) {
        for (int i = 0; i < num_of_results; i++) {
            if (i == selected_desktop) {
                std::string res = "<b>" + std::string(gtk_label_get_text(result_labels[i])) + "</b>";
                gtk_label_set_markup(result_labels[i], res.c_str());
            } else {
                gtk_label_set_text(result_labels[i], gtk_label_get_text(result_labels[i]));
            }
        }
    }
}

bool on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    /**
     * Handle key presses.
     * Returns TRUE when the key press is handled.
     */
    if (event->keyval == GDK_KEY_Escape) {
        clean_exit();
        return TRUE;
    } else if (num_of_results > 0 &&
        (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter)) {
        execlp("gtk-launch", "gtk-launch", result_desktops[selected_desktop]->id.c_str(), NULL);
        return TRUE;
    } else if (event->keyval == GDK_KEY_downarrow || event->keyval == GDK_KEY_Down) {
        if (++selected_desktop >= num_of_results) {
            selected_desktop = 0;
        }
        update_selected();
        return TRUE;
    } else if (event->keyval == GDK_KEY_uparrow || event->keyval == GDK_KEY_Up) {
        if (--selected_desktop < 0) {
            selected_desktop = num_of_results - 1;
            if (selected_desktop == -1) {
                selected_desktop = 0;
            }
        }
        update_selected();
        return TRUE;
    }
    return FALSE;
}

void on_search_changed(GObject* object) {
    /**
     * Called when the search bar text is changed.
     * Searches the .desktop entries, and sets the GUI based on the results
     */
    std::string query = std::string(gtk_entry_get_text(search_entry));
    search(query, all_desktops);
    num_of_results = 0;
    for (int i = 0; i < 5; i++) {
        if (i >= result.size()) {
            // If there isn't a result to put here
            gtk_widget_hide(GTK_WIDGET(result_labels[i]));
            gtk_widget_hide(GTK_WIDGET(result_images[i]));
        } else {
            num_of_results++;
            Desktop* entry = result.top();
            result.pop();
            result_desktops[i] = entry;

            // If there IS a result to put here
            gtk_widget_show(GTK_WIDGET(result_labels[i]));
            gtk_widget_show(GTK_WIDGET(result_images[i]));

            gtk_label_set_text(result_labels[i], entry->name.c_str());
            if (entry->icon_type == ICON_TYPE_FILE) {
                gtk_image_set_from_pixbuf(result_images[i], 
                    gdk_pixbuf_new_from_file_at_scale(entry->icon.c_str(), 48, 48, FALSE, NULL));
            } else if (entry->icon_type == ICON_TYPE_NAME) {
                gtk_image_set_from_icon_name(result_images[i], entry->icon.c_str(), GTK_ICON_SIZE_DIALOG);
            }
        }
    }
    if (selected_desktop >= num_of_results) {
        selected_desktop = num_of_results - 1;
        if (selected_desktop == -1) {
            selected_desktop = 0;
        }
    }
    update_selected();
}

void init_gui() {
    /**
     * Initializes the GUI
     */
    builder = gtk_builder_new_from_file("view.glade");

    window = GTK_WINDOW(gtk_builder_get_object(builder, "main"));
    gtk_window_set_decorated(window, FALSE);
    g_signal_connect(G_OBJECT(window), "focus-out-event", G_CALLBACK(clean_exit), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(clean_exit), NULL);

    search_entry = GTK_ENTRY(gtk_builder_get_object(builder, "input"));
    g_signal_connect(G_OBJECT(search_entry), "changed", G_CALLBACK(on_search_changed), NULL);
    g_signal_connect(G_OBJECT(search_entry), "key-press-event", G_CALLBACK(on_key_press), NULL);

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
    for (int i = 0; i < 5; i++) {
        gtk_widget_hide(GTK_WIDGET(result_labels[i]));
        gtk_widget_hide(GTK_WIDGET(result_images[i]));
    }
    gtk_main();

    return 0;
}
