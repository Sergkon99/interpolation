#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

    void setX(int min,int max);
    void setY(int min, int max);
    void setSize(int height, int weight);
    void setO();
    void calcScale();

    void draw(QPainter&);
    void draw();
    void drawAxes(QPainter&);
    void drawPoints(QPainter&, int, const QStringList&, const QStringList&);

    QString getStrAns(const std::vector<double>&);

    void solve(int, const QStringList&, const QStringList&);

private slots:
    void on_btn_Solve2_clicked();

private:
    Ui::MainWindow *ui;

    double f(double);
};

#endif // MAINWINDOW_H
