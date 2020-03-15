#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "libraries.h"

// Forward reference
class MainPage;
class SimWindow;
class AboutPage;
class SettingsPage;

// Main window where the program will start
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // Method to get proper background
    void resizeEvent(QResizeEvent *event) override;

signals:
    void stop_music();
    void play_music();

public slots:
    // Functions to navigate the widgets
    void go_to_sim();
    void go_to_about();
    void go_to_settings();
    void go_to_main();

private:
    QStackedWidget *stacked_widgets;
    std::vector<QLineEdit*> color_fields;
};

// Skeleton widget for pages
class Page : public QWidget
{
    Q_OBJECT
public:
    virtual ~Page();
};

// Main page widget consisting of main navigation buttons
class MainPage : public Page
{
    Q_OBJECT
public:
    MainPage(QWidget *parent = nullptr);

signals:
    void stop_music();
    void play_music();
};

// About page widget consisting of information
class AboutPage : public Page
{
    Q_OBJECT
public:
    AboutPage(QWidget *parent = nullptr);
};

// Settings page widget consisting of color and music settings
class SettingsPage : public Page
{
    Q_OBJECT
public:
    SettingsPage(QWidget *parent = nullptr);

    std::vector<QLineEdit*> get_color_fields() const;

private:
    std::vector<QLineEdit*> color_fields;
};

#endif // MAIN_WINDOW_H
