#include <gtk/gtk.h> // GTK is the GUI framework for this program.
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

int timeLeft = 90;
int score = 0;
std::string currentColor;
GtkWidget *titleLabel;
GtkWidget *colorLabel;
GtkWidget *timeLabel;
GtkWidget *scoreLabel;
GtkWidget *entry;
GtkWidget *restartButton;
guint timerId = 0; // Variable to store the timer ID

// Colors and their names
std::vector<std::string> colorNames = {"Red", "Blue", "Green", "Yellow", "Pink", "Orange", "Black", "Gray"};
std::vector<std::string> colorHex = {"#FF0000", "#0000FF", "#00FF00", "#FFFF00", "#FFC0CB", "#FFA500", "#000000", "#808080"};

// Function to apply CSS dynamically
void apply_css(GtkWidget *widget, const std::string &css) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css.c_str(), -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

// Function to update the game state
void next_color() {
    int textIndex = rand() % colorNames.size();
    int colorIndex = rand() % colorNames.size();

    currentColor = colorNames[colorIndex];
    gtk_label_set_text(GTK_LABEL(colorLabel), colorNames[textIndex].c_str());

    // Dynamically update the text color using CSS
    std::string css = "label { font-size: 90px; font-weight: bold; color: " + colorHex[colorIndex] + "; }";
    apply_css(colorLabel, css);

    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

// Timer callback
gboolean update_timer(gpointer data) {
    timeLeft--;
    std::string timeText = "Time left: " + std::to_string(timeLeft);
    gtk_label_set_text(GTK_LABEL(timeLabel), timeText.c_str());

    if (timeLeft <= 0) {
        gtk_label_set_text(GTK_LABEL(timeLabel), "Game Over!");
        gtk_widget_set_sensitive(entry, FALSE);
        return FALSE; // Stop the timer
    }

    return TRUE; // Continue the timer
}

// Check the user's input
void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    const char *input = gtk_entry_get_text(entry);
    if (currentColor == input) {
        score++;
    }
    else if (score <= 0){ // Make sure it does not go to the negatives.
        score = 0; 
    }
    else {
        score--;
    }

    std::string scoreText = "Score: " + std::to_string(score);
    gtk_label_set_text(GTK_LABEL(scoreLabel), scoreText.c_str());
    next_color();

    // Start the timer when the user presses enter for the first time
    if (timeLeft == 90 && timerId == 0) {
        timerId = g_timeout_add_seconds(1, update_timer, NULL);
    }
}

// Restart the game
void restart_game(GtkButton *button, gpointer user_data) {
    // Stop any existing timer
    if (timerId != 0) {
        g_source_remove(timerId);
        timerId = 0;
    }

    timeLeft = 90;
    score = 0;

    gtk_label_set_text(GTK_LABEL(scoreLabel), "Score: 0");
    gtk_label_set_text(GTK_LABEL(timeLabel), "Time left: 90");
    gtk_widget_set_sensitive(entry, TRUE);

    next_color();
}
// This will set up the application layout and the style. Also this is what starts up first is the main function. :) Very cool right.
//Also without main the program would not work its important to have the main function. 
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    srand(time(0));

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "The Color Game Remaster"); // The title to the Application.
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300); //The size of the application. 
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Make window non-resizable

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Add a bigger heading. 
    titleLabel = gtk_label_new("The Color Game Remaster:"); // The title label. :) 
    gtk_widget_set_size_request(titleLabel, 300, 50);
    gtk_box_pack_start(GTK_BOX(vbox), titleLabel, TRUE, TRUE, 5);
    apply_css(titleLabel, "label { font-size: 56px; font-weight: bold; text-decoration: underline; }"); // You can use CSS to apply styles for example changing the font size.
    timeLabel = gtk_label_new("Time left: 90");
    gtk_box_pack_start(GTK_BOX(vbox), timeLabel, TRUE, TRUE, 5);
    apply_css(timeLabel, "label { font-size: 32px; color: #000000; }");

    scoreLabel = gtk_label_new("Score: 0");
    gtk_box_pack_start(GTK_BOX(vbox), scoreLabel, TRUE, TRUE, 5);
    apply_css(scoreLabel, "label { font-size: 32px; color: #000000; }");

    colorLabel = gtk_label_new("Type the color!");
    gtk_widget_set_size_request(colorLabel, 300, 70);
    gtk_box_pack_start(GTK_BOX(vbox), colorLabel, TRUE, TRUE, 5);



    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 5);
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), NULL);

    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), buttonBox, TRUE, TRUE, 5);

    restartButton = gtk_button_new_with_label("Restart");
    gtk_box_pack_start(GTK_BOX(buttonBox), restartButton, TRUE, TRUE, 90);
    g_signal_connect(restartButton, "clicked", G_CALLBACK(restart_game), NULL);

    // Make the button text larger
    apply_css(restartButton, "button { font-size: 48px; }");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    next_color();

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
