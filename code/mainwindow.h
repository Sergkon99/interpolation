#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QtDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QMessageBox>
#include <QMouseEvent>

#include <math.h>
#include "equationsystem.h"
#include "polynom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Polynom p;
    QPixmap * map;
    QPainter * painter;

    bool wasSolve = false;
    int count;
    std::vector<double>x;
    std::vector<double>y;

    int xStep;
    int yStep;

    int xMin;
    int xMax;
    int yMin;
    int yMax;

    int h;
    int w;
    double oX;
    double oY;

    double scaleX;
    double scaleY;

    bool setPoint;
    bool showGrid;
    bool onlyInt;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void init(int, int);

    void setCount(int);
    void setX(int,int);
    void setY(int, int);
    void setSize(int, int);
    void setO();
    void calcScale();

    void clearPainter(QPainter&);
    void draw(QPainter&);
    void draw();
    void drawGrid();
    void drawAxes(QPainter&);
    void drawPoints(QPainter&, int, const std::vector<double>&, const std::vector<double>&);
    void drawLines(QPainter&, int, const std::vector<double>&, const std::vector<double>&);
    void drawGraph(QPainter&, const Polynom&);
    void updateGraph();

    void clearPainter();
    void drawAxes();
    void drawPoints();
    void drawLines();
    void drawGraph(const Polynom&);

    QString getStrAns(const std::vector<double>&);

    void solve(int, const QStringList&, const QStringList&);
    void solve(int, const std::vector<double>&, const std::vector<double>&);
    void solve();

private slots:
    void on_btn_Solve2_clicked();

    void on_btn_chooseFile_clicked();

    void on_btn_Solve1_clicked();

    void on_pb_chX_clicked();

    void on_pb_chY_clicked();

    void on_pb_chX_minus_clicked();

    void on_pb_chY_minus_clicked();

    void on_pb_default_clicked();

    void on_cb_setPoint_stateChanged(int arg1);

    void on_cb_showGrid_stateChanged(int arg1);

    void on_cb_onlyInt_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    void setInputData();
    double f(double);

protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // MAINWINDOW_H
