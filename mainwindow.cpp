#include "mainwindow.h"
#include "ui_mainwindow.h"

void setFixedGeometry(QWidget* w, int ax, int ay, int aw, int ah){
    w->setGeometry(ax, ay, aw, ah);
    w->setMinimumSize(aw, ah);
    w->setMaximumSize(aw, ah);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(QString("Tournament grid"));
    createInput();
}

void MainWindow::createInput(){
    setFixedGeometry(this, 10, 30, 210, 230);
    QTextEdit *edit = new QTextEdit(this);
    edit->setGeometry(5, 5, 200, 200);
    QPushButton *btn = new QPushButton(this);
    btn->setGeometry(4, 205, 202, 25);
    btn->setText(QString("Create grid"));
    connect(btn, &QPushButton::clicked, this, [=](){ on_btn_click(edit, btn);});
    edit->show();
    btn->show();
}

void MainWindow::on_btn_click(QTextEdit* edit, QPushButton* btn){
    players = edit->toPlainText().split('\n');
    if(players.front() == "")
        players.pop_front();
    if(players.back() == "")
        players.pop_back();
    if(players.size() < 2)
        return;
    std::srand(time(NULL));
    std::random_shuffle(players.begin(), players.end());
    delete edit;
    delete btn;
    createGrid();
}

void MainWindow::createGrid(){
    int count = players.size();
    stages = std::ceil(log2(count)) + 1;
    setFixedGeometry(this, 10, 30, stages*BUTTON_W, BUTTON_H*(count*2-1));

    createButtons(this);
    createVLines(this);
    createHLines(this);
}

void MainWindow::createButtons(QWidget* parent){
    //creating
    clean_mem(buttons);
    buttons = QVector<QVector<QPushButton*>>(stages);
    for(int stage=0, c=players.size(); stage<stages; ++stage, c=(c+1)/2){
        buttons[stage] = QVector<QPushButton*>(c);
        for(int i=0; i<c; ++i){
            buttons[stage][i] = new QPushButton(parent);
        }
    }

    // binding
    for(int stage=0; stage<stages-1; ++stage){
        for(int i=0; i<buttons[stage].size(); ++i){
            QPushButton *btn = buttons[stage][i];
            QPushButton *next_btn = buttons[stage+1][i/2];
            connect(btn, &QPushButton::clicked, this, [=](){ next_btn->setText(btn->text());});
        }
    }

    for(int stage=0; stage<stages-1; ++stage){
        int stg = stage;
        QPushButton *btn = buttons[stage].back();
        while(buttons[++stage].size()%2){
            if(stage+1 >= stages)
                break;
            QPushButton *next_btn = buttons[stage+1].back();
            connect(btn, &QPushButton::clicked, this, [=](){ next_btn->setText(btn->text());});
        }
        stage = stg;
        if(buttons[stage].size()%2)
            continue;
        btn = buttons[stage][buttons[stage].size()-2];
        while(buttons[++stage].size()%2){
            if(stage+1 >= stages)
                break;
            QPushButton *next_btn = buttons[stage+1].back();
            connect(btn, &QPushButton::clicked, this, [=](){ next_btn->setText(btn->text());});
        }
        stage = stg;
    }

    // geometry
    for(int i=0; i<buttons[0].size(); ++i){
        buttons[0][i]->setGeometry(0, BUTTON_H*i*2, BUTTON_W, BUTTON_H);
        buttons[0][i]->show();
    }
    for(int i=1; i<buttons.size(); ++i){
        for(int j=0; j<buttons[i].size(); ++j){
            auto btn = buttons[i][j];
            auto prev = buttons[i-1][j*2];
            if(buttons[i-1].size()%2 && j==buttons[i].size()-1)
                btn->setGeometry(prev->x()+BUTTON_W, prev->y(), BUTTON_W, BUTTON_H);
            else
                btn->setGeometry(prev->x()+BUTTON_W, prev->y() + (buttons[i-1][j*2+1]->y() - prev->y())/2, BUTTON_W, BUTTON_H);
            btn->show();
        }
    }

    // text
    for(int i=0; i<buttons[0].size(); ++i)
        buttons[0][i]->setText(players[i]);

    if(buttons[0].size()%2){
        int stage = 0;
        auto btn = buttons[0].back();
        while(buttons[stage].size()%2)
            buttons[++stage].back()->setText(btn->text());
    }
}

void MainWindow::createVLines(QWidget* parent){
    clean_mem(vLines);
    vLines = QVector<QVector<QFrame*>>(stages);
    for(int stage=0, c=players.size(); stage<stages-1; ++stage, c = (c+1)/2){
        vLines[stage] = QVector<QFrame*>(c/2);
        int ax = buttons[stage][0]->x() + buttons[stage][0]->width()/2 - 5;
        for(int i=0; i<buttons[stage].size()/2; ++i){
            int ay = buttons[stage][i*2]->y() + buttons[stage][i*2]->height() - 2;
            int s = buttons[stage][i*2+1]->y() - buttons[stage][i*2]->y() - BUTTON_H + 4;
            QFrame *line = createVLine(parent, ax, ay, s);
            line->show();
            vLines[stage][i] = line;
        }
    }
}

void MainWindow::createHLines(QWidget *parent){
    clean_mem(hLines);
    hLines = QVector<QVector<QFrame*>>(stages);
    for(int stage=0, c=(players.size()+1)/2; stage<stages-1; ++stage, c = (c+1)/2){
        hLines[stage] = QVector<QFrame*>(c);
        int ax = buttons[stage].front()->x() + BUTTON_W/2 + 3;
        int p = (buttons[stage].size()+1)/2;
        for(int i=0, ay, s; i<p; ++i){
            if(buttons[stage].size()%2 == 1 && i==p-1){
                ax += BUTTON_W/2 - 4;
                ay = buttons[stage].back()->y() + BUTTON_H/2 - 5;
                s = buttons[stage+1].back()->x() - buttons[stage].back()->x() - BUTTON_W + 3;
            } else{
                ay = buttons[stage][i*2]->y() + (buttons[stage][i*2+1]->y() - buttons[stage][i*2]->y())/2 + 5;
                s = buttons[stage+1].front()->x() - ax + 1;
            }
            QFrame *line = createHLine(parent, ax, ay, s);
            line->show();
            hLines[stage][i] = line;
        }
    }
}

QFrame* MainWindow::createVLine(QWidget *parent, int ax, int ay, int s){
    QFrame *line = new QFrame(parent);
    line->setGeometry(ax, ay, 10, s);
    line->setFrameShadow(QFrame::Plain);
    line->setLineWidth(5);
    line->setMidLineWidth(5);
    line->setFrameShape(QFrame::VLine);
    return line;
}

QFrame* MainWindow::createHLine(QWidget *parent, int ax, int ay, int s){
    QFrame *line = new QFrame(parent);
    line->setGeometry(ax, ay, s, 10);
    line->setFrameShadow(QFrame::Plain);
    line->setLineWidth(5);
    line->setMidLineWidth(5);
    line->setFrameShape(QFrame::HLine);
    return line;
}

MainWindow::~MainWindow(){
    clean_mem(buttons);
    clean_mem(vLines);
    clean_mem(hLines);

    delete ui;
}
