#include "main_window.h"
#include "graph_window.h"

const QSize BTN_SIZE = QSize(700, 75);
const QString BTN_FONT = "font: 25pt; color: black;";
const QColor DEFAULT_NODE_COLOR = "cyan";
const QColor DEFAULT_EDGE_COLOR = "black";
const QColor DEFAULT_ALGO_COLOR = "magenta";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setParent(parent);
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("Algorithm Archives");

    QWidget *page = new QWidget;
    setCentralWidget(page);
    QGridLayout *layout = new QGridLayout;
    page->setLayout(layout);
    stacked_widgets = new QStackedWidget;
    layout->addWidget(stacked_widgets);

    // main page, widget index = 0
    MainPage *main_page = new MainPage(this);
    stacked_widgets->addWidget(main_page);

    // about page, widget index = 1
    AboutPage *about_page = new AboutPage(this);
    stacked_widgets->addWidget(about_page);

    // settings page, widget index = 2
    SettingsPage *settings_page = new SettingsPage(this);
    stacked_widgets->addWidget(settings_page);

    // music
    connect(this, SIGNAL(stop_music()), main_page, SIGNAL(stop_music()));
    connect(this, SIGNAL(play_music()), main_page, SIGNAL(play_music()));

    // background image
    QPixmap background(":/img/bridges.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    setPalette(palette);

    color_fields = settings_page->get_color_fields();
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < color_fields.size(); ++i)
    {
        delete color_fields[i];
        color_fields[i] = nullptr;
    }

    delete stacked_widgets;
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

void MainWindow::go_to_sim()
{
    GraphWindow *gw = new GraphWindow(this, color_fields);
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

Page::~Page() {};

MainPage::MainPage(QWidget *parent)
{
    setParent(parent);

    QGridLayout *layout = new QGridLayout;
    setLayout(layout);

    // title
    QLabel *title = new QLabel("Algorithm Archives");
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("QLabel { font: 130pt; color: blue;}");
    layout->addWidget(title, 0, 0);

    // buttons
    std::vector<QPushButton*> buttons;
    QPushButton *simulate = new QPushButton("Simulate");
    QPushButton *about = new QPushButton("About");
    QPushButton *settings = new QPushButton("Settings");
    QPushButton *quit = new QPushButton("Quit");

    connect(simulate, SIGNAL(clicked()), parentWidget(), SLOT(go_to_sim()));
    connect(about, SIGNAL(clicked()), parentWidget(), SLOT(go_to_about()));
    connect(settings, SIGNAL(clicked()), parentWidget(), SLOT(go_to_settings()));
    connect(quit, SIGNAL(clicked()), parentWidget(), SLOT(close()));

    buttons.push_back(simulate);
    buttons.push_back(about);
    buttons.push_back(settings);
    buttons.push_back(quit);

    int i = 0;
    for(auto it : buttons)
    {
        layout->addWidget(it, i + 1, 0);
        ++i;
        it->setFixedSize(BTN_SIZE);
        it->setStyleSheet(BTN_FONT);
        layout->setAlignment(it, Qt::AlignHCenter);
    }

    // background music
    QMediaPlayer *music = new QMediaPlayer;
    music->setMedia(QUrl("qrc:/audio/main_music.mp3"));
    music->play();

    connect(this, SIGNAL(stop_music()), music, SLOT(stop()));
    connect(this, SIGNAL(play_music()), music, SLOT(play()));
}

AboutPage::AboutPage(QWidget *parent)
{
    setParent(parent);

    QGridLayout *layout = new QGridLayout;
    setLayout(layout);

    QPushButton *back = new QPushButton(tr("Back"));

    connect(back, SIGNAL(clicked()), parentWidget(), SLOT(go_to_main()));

    layout->addWidget(back, 1, 0);
    back->setFixedSize(BTN_SIZE);
    back->setStyleSheet(BTN_FONT);
    layout->setAlignment(back, Qt::AlignHCenter);

    // contents
    QLabel *text = new QLabel;
    text->setText("Programmers: Gilbert Neuner, Daryan Sugandhi, Esam Izzat, Andrew Lybianto\n"
                  "Description: This project executes Depth First Search and Breadth First Search on graphs.\n"
                  "Instructions: https://github.com/gilbertneuner/algorithm_archives/blob/master/README.md");
    text->setStyleSheet("QLabel { font: 40pt; color: black; background-color: white;}");
    text->setAlignment(Qt::AlignVCenter);
    text->setWordWrap(true);
    layout->addWidget(text, 0, 0);
}

SettingsPage::SettingsPage(QWidget *parent)
{
    setParent(parent);

    QGridLayout *layout = new QGridLayout;
    setLayout(layout);

    QLabel *node_color_name = new QLabel;
    node_color_name->setText("Node color:");
    node_color_name->setStyleSheet("QLabel { font: 20pt; color: black; background-color: white;}");
    QLineEdit *node_color = new QLineEdit;
    node_color->setText(DEFAULT_NODE_COLOR.name());

    QLabel *edge_color_name = new QLabel;
    edge_color_name->setText("Edge color:");
    edge_color_name->setStyleSheet("QLabel { font: 20pt; color: black; background-color: white;}");
    QLineEdit *edge_color = new QLineEdit;
    edge_color->setText(DEFAULT_EDGE_COLOR.name());

    QLabel *algo_color_name = new QLabel;
    algo_color_name->setText("Algorithm color:");
    algo_color_name->setStyleSheet("QLabel { font: 20pt; color: black; background-color: white;}");
    QLineEdit *algo_color = new QLineEdit;
    algo_color->setText(DEFAULT_ALGO_COLOR.name());

    // index: 0: node, 1: edge, 2: algorithm
    color_fields.push_back(node_color);
    color_fields.push_back(edge_color);
    color_fields.push_back(algo_color);

    layout->addWidget(node_color_name, 0, 0);
    layout->addWidget(node_color, 1, 0);
    layout->addWidget(edge_color_name, 2, 0);
    layout->addWidget(edge_color, 3, 0);
    layout->addWidget(algo_color_name, 4, 0);
    layout->addWidget(algo_color, 5, 0);

    // buttons
    std::vector<QPushButton*> buttons;
    QPushButton *stop = new QPushButton("Stop music");
    QPushButton *play = new QPushButton("Play music");

    QPushButton *back = new QPushButton(tr("Back"));

    connect(stop, SIGNAL(clicked()), parentWidget(), SIGNAL(stop_music()));
    connect(play, SIGNAL(clicked()), parentWidget(), SIGNAL(play_music()));
    connect(back, SIGNAL(clicked()), parentWidget(), SLOT(go_to_main()));

    buttons.push_back(stop);
    buttons.push_back(play);
    buttons.push_back(back);

    int i = 6;
    for(auto it :buttons)
    {
        layout->addWidget(it, i + 1, 0);
        i++;
        it->setFixedSize(BTN_SIZE);
        it->setStyleSheet(BTN_FONT);
        layout->setAlignment(it, Qt::AlignHCenter);
    }
}

std::vector<QLineEdit*> SettingsPage::get_color_fields() const
{
    return color_fields;
}
