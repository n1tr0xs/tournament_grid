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
    if(players.back() == "")
        players.pop_back();
    if(players.size() < 2)
        return;
    if(players.front() == "" && players.size() == 1)
        return;
    delete edit;
    delete btn;
    createGrid();
}

void MainWindow::createGrid(){
    std::random_shuffle(players.begin(), players.end());
    const int count = players.size();
    stages = std::ceil(log2(count)) + 1;
    setFixedGeometry(this, 10, 30, stages*BUTTON_W, BUTTON_H*(count*2-1));

    createButtons(this, count);
    // buttons geometry
    for(size_t i=0; i<buttons[0].size(); ++i){
        buttons[0][i]->setGeometry(0, BUTTON_H*i*2, BUTTON_W, BUTTON_H);
        buttons[0][i]->show();
    }
    for(size_t i=1; i<buttons.size(); ++i){
        for(size_t j=0; j<buttons[i].size(); ++j){
            auto btn = buttons[i][j];
            auto prev = buttons[i-1][j*2];
            if(buttons[i-1].size()%2 && j==buttons[i].size()-1)
                btn->setGeometry(prev->x()+BUTTON_W, prev->y(), BUTTON_W, BUTTON_H);
            else
                btn->setGeometry(prev->x()+BUTTON_W, prev->y() + (buttons[i-1][j*2+1]->y() - prev->y())/2, BUTTON_W, BUTTON_H);
            buttons[i][j]->show();
        }
    }

    // setting players names on 1st stage
    for(size_t i=0; i<buttons[0].size(); ++i)
        buttons[0][i]->setText(players[i]);
    // setting "autowinner" if odd number of players
    if(buttons[0].size()%2){
        int stage = 0;
        auto btn = buttons[0].back();
        while(buttons[stage].size()%2)
            buttons[++stage].back()->setText(btn->text());
    }

    // buttons binds
    for(int stage=0; stage<stages-1; ++stage){
        for(size_t i=0; i<buttons[stage].size(); ++i){
            QPushButton *btn = buttons[stage][i];
            QPushButton *next_btn = buttons[stage+1][i/2];
            connect(btn, &QPushButton::clicked, this, [=](){ next_btn->setText(btn->text());});
        }
    }

    // still button binds
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

    // creating, placing vertical lines
    vLines = vector<vector<QFrame*>>(stages);
    for(int stage=0, c=players.size(); stage<stages-1; ++stage, c = (c+1)/2){
        vLines[stage] = vector<QFrame*>(c/2);
        int ax = buttons[stage][0]->x() + buttons[stage][0]->width()/2 - 5;
        for(size_t i=0; i<buttons[stage].size()/2; ++i){
            int ay = buttons[stage][i*2]->y() + buttons[stage][i*2]->height() - 2;
            int s = buttons[stage][i*2+1]->y() - buttons[stage][i*2]->y() - BUTTON_H + 4;
            QFrame *line = createVLine(this, ax, ay, s);
            line->show();
            vLines[stage][i] = line;
        }
    }

    // creating, placing horizontal lines
    hLines = vector<vector<QFrame*>>(stages);
    for(int stage=0, c=(players.size()+1)/2; stage<stages-1; ++stage, c = (c+1)/2){
        hLines[stage] = vector<QFrame*>(c);
        int ax = buttons[stage].front()->x() + BUTTON_W/2 + 3;
        int p = (buttons[stage].size()+1)/2;
        for(int i=0; i<p; ++i){
            int ay, s;
            if(buttons[stage].size()%2 == 1 && i==p-1){
                ax += BUTTON_W/2 - 4;
                ay = buttons[stage].back()->y() + BUTTON_H/2 - 5;
                s = buttons[stage+1].back()->x() - buttons[stage].back()->x() - BUTTON_W + 3;
            } else{
                ay = buttons[stage][i*2]->y() + (buttons[stage][i*2+1]->y() - buttons[stage][i*2]->y())/2 + 5;
                s = buttons[stage+1].front()->x() - ax + 1;
            }
            QFrame *line = createHLine(this, ax, ay, s);
            line->show();
            hLines[stage][i] = line;
        }
    }
}

void MainWindow::createButtons(QWidget* parent, int c){
    clean_mem(buttons);
    buttons = vector<vector<QPushButton*>>(stages);
    for(int stage=0; stage<stages; ++stage, c=(c+1)/2){
        buttons[stage] = vector<QPushButton*>(c);
        for(int i=0; i<c; ++i){
            buttons[stage][i] = new QPushButton(parent);
            buttons[stage][i]->hide();
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
