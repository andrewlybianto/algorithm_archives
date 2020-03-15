#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "libraries.h"

// forward reference
class MainPage;
class SimWindow;
class AboutPage;
class SettingsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void resizeEvent(QResizeEvent *event) override;

signals:
    void stop_music();
    void play_music();

public slots:
    void go_to_sim();
    void go_to_about();
    void go_to_settings();
    void go_to_main();

private:
    QStackedWidget *stacked_widgets;
    std::vector<QLineEdit*> color_fields;
};

class Page : public QWidget
{
    Q_OBJECT
public:
    virtual ~Page();
};

class MainPage : public Page
{
    Q_OBJECT
public:
    MainPage(QWidget *parent = nullptr);

signals:
    void stop_music();
    void play_music();
};

class AboutPage : public Page
{
    Q_OBJECT
public:
    AboutPage(QWidget *parent = nullptr);
};

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
