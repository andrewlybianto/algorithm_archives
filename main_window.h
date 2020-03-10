#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "libraries.h"

// forward declaration
class MainPage;
class SimWindow;
class AboutPage;
class SettingsPage;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void resizeEvent(QResizeEvent *event) override;

public slots:
    void go_to_sim();
    void go_to_about();
    void go_to_settings();
    void go_to_main();
    void destroy();

private:
    QStackedWidget *stacked_widgets;
    QLayout *layout;

};

class Page : public QWidget
{
    Q_OBJECT
public:
    QWidget* get_content() const;
    std::vector<QPushButton*>* get_buttons() const;
    virtual ~Page();

protected:
    QWidget *content;
    QGridLayout *layout;
    std::vector<QPushButton*> *buttons;

};

class MainPage : public Page
{
    Q_OBJECT
public:
    MainPage();
    ~MainPage();

    // accessor to content
    QMediaPlayer* get_music() const;

private:
    QMediaPlayer *music;
};

class AboutPage : public Page
{
    Q_OBJECT
public:
    AboutPage();
    ~AboutPage();
};

class SettingsPage : public Page
{
    Q_OBJECT
public:
    SettingsPage();
    ~SettingsPage();
};

#endif // MAIN_WINDOW_H
