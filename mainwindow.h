#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QStringList>
#include <vector>
#include <random>
#include <ctime>
using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    // vars
    QStringList players;
    int stages;
    int BUTTON_H=20, BUTTON_W=110;
    vector<vector<QPushButton*>> buttons;
    vector<vector<QFrame*>> vLines;
    vector<vector<QFrame*>> hLines;
    // funcs
    void createInput();
    void createGrid();
    void on_btn_click(QTextEdit* edit, QPushButton* btn);
    QFrame* createVLine(QWidget* parent, int ax, int ay, int s);
    QFrame* createHLine(QWidget* parent, int ax, int ay, int s);
};
#endif // MAINWINDOW_H
