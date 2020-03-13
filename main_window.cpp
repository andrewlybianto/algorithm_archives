#include "main_window.h"
#include "graph_window.h"

const QSize BTN_SIZE = QSize(700, 75);
const QString BTN_FONT = "font: 25pt; color: black;";

MainWindow::MainWindow(QWidget *parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("Algorithm Archives");

    layout = new QGridLayout;
    stacked_widgets = new QStackedWidget;
    layout->addWidget(stacked_widgets);
    setLayout(layout);

    // main page, widget index = 0
    MainPage *main_page = new MainPage(this);
    stacked_widgets->addWidget(main_page->get_content());
    std::vector<QPushButton*> *main_buttons = main_page->get_buttons();

    // main page button functions
    connect(main_buttons->at(0), SIGNAL(clicked()), this, SLOT(go_to_sim()));
    connect(main_buttons->at(1), SIGNAL(clicked()), this, SLOT(go_to_about()));
    connect(main_buttons->at(2), SIGNAL(clicked()), this, SLOT(go_to_settings()));
    connect(main_buttons->at(3), SIGNAL(clicked()), this, SLOT(destroy()));

    // about page, widget index = 1
    AboutPage *about = new AboutPage(this);
    stacked_widgets->addWidget(about->get_content());
    std::vector<QPushButton*> *about_buttons = about->get_buttons();

    // about page button functions
    connect(about_buttons->at(0), SIGNAL(clicked()), this, SLOT(go_to_main()));

    // settings page, widget index = 2
    SettingsPage *settings = new SettingsPage(this);
    object_color = settings->get_color();
    stacked_widgets->addWidget(settings->get_content());
    std::vector<QPushButton*> *settings_buttons = settings->get_buttons();

    // settings page button functions
    connect(settings_buttons->at(0), SIGNAL(clicked()), main_page->get_music(), SLOT(stop()));
    connect(settings_buttons->at(1), SIGNAL(clicked()), main_page->get_music(), SLOT(play()));
    connect(settings_buttons->at(2), SIGNAL(clicked()), this, SLOT(go_to_main()));



    // background image
    QPixmap background(":/img/bridges.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);


}

MainWindow::~MainWindow()
{
    delete stacked_widgets;
    delete layout;
    close();
}

std::vector<QLineEdit*>* MainWindow::get_color() const
{
    return object_color;
}

// Override method to get proper background
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QPixmap background(":/img/bridges.jpg");
    background = background.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette();
    p.setBrush(QPalette::Background, background);
    setPalette(p);
}

void MainWindow::destroy()
{
    this->~MainWindow();
}

void MainWindow::go_to_sim()
{
    GraphWindow *gw = new GraphWindow(nullptr, get_color());
    gw->show();
}

void MainWindow::go_to_about()
{
    stacked_widgets->setCurrentIndex(1);
}

void MainWindow::go_to_settings()
{
    stacked_widgets->setCurrentIndex(2);
}

void MainWindow::go_to_main()
{
    stacked_widgets->setCurrentIndex(0);
}

Page::Page(QWidget *par)
{
    parent = par;
}

Page::~Page()
{
    delete buttons;
    delete layout;
    delete content;

}

QWidget* Page::get_content() const
{
    return content;
}

std::vector<QPushButton*>* Page::get_buttons() const
{
    return buttons;
}

MainPage::MainPage(QWidget *parent)
    : Page(parent)
{
    content = new QWidget;
    layout = new QGridLayout(content);

    // title
    QLabel *title = new QLabel("Algorithm Archives");
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("QLabel { font: 130pt; color: blue;}");
    layout->addWidget(title, 0, 0);

    // buttons
    buttons = new std::vector<QPushButton*>();
    QPushButton *simulate = new QPushButton("Simulate");
    QPushButton *about = new QPushButton("About");
    QPushButton *settings = new QPushButton("Settings");
    QPushButton *quit = new QPushButton("Quit");

    buttons->push_back(simulate);
    buttons->push_back(about);
    buttons->push_back(settings);
    buttons->push_back(quit);

    int i = 0;
    for(auto it = buttons->begin(); it != buttons->end(); ++it)
    {
        layout->addWidget(*it, i + 1, 0);
        i++;
        (*it)->setFixedSize(BTN_SIZE);
        (*it)->setStyleSheet(BTN_FONT);
        layout->setAlignment(*it, Qt::AlignHCenter);
    }

    // background music
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/audio/main_music.mp3"));
    music->play();

}

MainPage::~MainPage()
{

}

QMediaPlayer* MainPage::get_music() const
{
    return music;
}

AboutPage::AboutPage(QWidget *parent)
    : Page(parent)
{
    content = new QWidget;
    layout = new QGridLayout(content);

    // buttons
    buttons = new std::vector<QPushButton*>();
    QPushButton *back = new QPushButton(tr("Back"));

    buttons->push_back(back);

    int i = 0;
    for(auto it = buttons->begin(); it != buttons->end(); ++it)
    {
        layout->addWidget(*it, i + 1, 0);
        i++;
        (*it)->setFixedSize(BTN_SIZE);
        (*it)->setStyleSheet(BTN_FONT);
        layout->setAlignment(*it, Qt::AlignHCenter);
    }

    // contents
    QLabel *text = new QLabel();
    text->setText("Programmers: Gilbert Neuner, Daryan Sugandhi, Esam Izzat, Andrew Lybianto\n"
                  "Description: This project executes Depth First Search and Breadth First Search on a graph inputted by the user.");
    text->setStyleSheet("QLabel { font: 40pt; color: black; background-color: white;}");
    text->setAlignment(Qt::AlignVCenter);
    text->setWordWrap(true);
    layout->addWidget(text, 0, 0);
}

AboutPage::~AboutPage()
{

}

SettingsPage::SettingsPage(QWidget *parent)
    : Page(parent)
{
    content = new QWidget;
    layout = new QGridLayout(content);

    QLabel *node_info = new QLabel();
    node_info->setText("Node color:");
    node_info->setStyleSheet("QLabel { font: 20pt; color: black; background-color: white;}");
    QLineEdit *node_color = new QLineEdit;
    node_color->setText(DEFAULT_NODE_COLOR.name());

    QLabel *edge_info = new QLabel();
    edge_info->setText("Edge color:");
    edge_info->setStyleSheet("QLabel { font: 20pt; color: black; background-color: white;}");
    QLineEdit *edge_color = new QLineEdit;
    edge_color->setText(DEFAULT_EDGE_COLOR.name());

    QLabel *algo_info = new QLabel();
    algo_info->setText("Algorithm color:");
    algo_info->setStyleSheet("QLabel { font: 20pt; color: black; background-color: white;}");
    QLineEdit *algo_color = new QLineEdit;
    algo_color->setText(DEFAULT_ALGO_COLOR.name());

    color = new std::vector<QLineEdit*>();

    // index: 0: node, 1: edge, 2: algorithm
    color->push_back(node_color);
    color->push_back(edge_color);
    color->push_back(algo_color);

    layout->addWidget(node_info, 0, 0);
    layout->addWidget(node_color, 1, 0);
    layout->addWidget(edge_info, 2, 0);
    layout->addWidget(edge_color, 3, 0);
    layout->addWidget(algo_info, 4, 0);
    layout->addWidget(algo_color, 5, 0);

    // buttons
    buttons = new std::vector<QPushButton*>();
    QPushButton *mute = new QPushButton("Stop music");
    QPushButton *unmute = new QPushButton("Play music");

    QPushButton *back = new QPushButton(tr("Back"));


    buttons->push_back(mute);
    buttons->push_back(unmute);
    buttons->push_back(back);

    int i = 6;
    for(auto it = buttons->begin(); it != buttons->end(); ++it)
    {
        layout->addWidget(*it, i + 1, 0);
        i++;
        (*it)->setFixedSize(BTN_SIZE);
        (*it)->setStyleSheet(BTN_FONT);
        layout->setAlignment(*it, Qt::AlignHCenter);
    }
}

SettingsPage::~SettingsPage()
{

}

std::vector<QLineEdit*>* SettingsPage::get_color() const
{
    return color;
}
