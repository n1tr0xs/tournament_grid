#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QStringList>
#include <QVector>
#include <random>
#include <ctime>
#include "funcs.h"

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
    QVector<QVector<QPushButton*>> buttons;
    QVector<QVector<QFrame*>> vLines;
    QVector<QVector<QFrame*>> hLines;
    // funcs
    void on_btn_click(QTextEdit* edit, QPushButton* btn);
    void createInput();
    void createGrid();
    void createButtons(QWidget* parent);
    void createVLines(QWidget* parent);
    void createHLines(QWidget* parent);
    QFrame* createVLine(QWidget* parent, int ax, int ay, int s);
    QFrame* createHLine(QWidget* parent, int ax, int ay, int s);
};
#endif // MAINWINDOW_H
