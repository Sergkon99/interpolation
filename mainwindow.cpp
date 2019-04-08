#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QtDebug>
#include <QStandardItemModel>
#include <QStandardItem>

#define all(x) x.begin(),x.end()
const double eps = 1e-9;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setSize(ui->output->height(),
            ui->output->width());
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    setX(-10, 10);
    setY(-10, 10);
    setO();
    calcScale();
}

void MainWindow::setX(int min, int max){
    xMin = min;
    xMax = max;
}

void MainWindow::setY(int min, int max){
    yMin = min;
    yMax = max;
}

void MainWindow::setSize(int height, int weight){
    h = height;
    w = weight;
}

void MainWindow::setO(){
    oX = fabs(xMin);
    oY = yMax;
}

void MainWindow::calcScale(){
    scaleX = 1.0 * w / (xMax - xMin);
    scaleY = 1.0 * h / (yMax - yMin);
}

QString MainWindow::getStrAns(const std::vector<double> &res){
    bool firstDigit = true;
    QString strAns = "L<sub>n</sub>(x) = ";

    for(int i = res.size()-1;i>=0;i--){
        if(fabs(res[i]) < eps)
            continue;
        QString tmp="";

        if(firstDigit){
            firstDigit = false;
            if(res[i]<0)
                tmp+="-";
        }
        else{
            if(res[i]<0)
                tmp+="-";
            else
                tmp+="+";
        }

        if(fabs(res[i]-1) > eps)
            tmp+=QString::number(fabs(res[i]),'g',2);

        if(i!=0){
            if(i==1)
                tmp+="x";
            else
                tmp+="x<sup>" + QString::number(i)+ "</sup>";
        }
        strAns+=tmp;
    }

    return strAns;
}

double MainWindow::f(double x){
    return p(x);
}

void MainWindow::drawAxes(QPainter& painter){
    painter.eraseRect(0,0,w,h);

    painter.drawPoint(oX, oY);

    QLine l1(oX*scaleX,0,oX*scaleX,h);
    QLine l2(0,oY*scaleY,w,oY*scaleY);

    painter.drawLine(l1);
    painter.drawLine(l2);

    painter.setPen(QPen(Qt::black,3));

    for(int i = xMin;i <= xMax;i++)
        painter.drawPoint((oX+i)*scaleX, (oY)*scaleY);

    for(int i = yMin;i <= yMax;i++)
        painter.drawPoint(oX*scaleX, (i+oY)*scaleY);

    painter.setPen(QPen(Qt::black,1));
}

void MainWindow::draw(){
    QPixmap map(w, h);
    QPainter paint;

    paint.begin(&map);

    drawAxes(paint);

    QPainterPath path;

    paint.setPen(QPen(Qt::blue,1));

    double step = 0.01;
    bool first = true;

    for(double i = xMin; i <= xMax; i+=step){
        if(first){
           path.moveTo((i+oX)*scaleX,(oY-f(i))*scaleY);
           first = false;
        }
        else {
            path.lineTo((i+oX)*scaleX,(oY-f(i))*scaleY);
        }
    }

    paint.drawPath(path);

    paint.end();

    ui->output->setPixmap(map);

}

void MainWindow::draw(QPainter& paint){
    drawAxes(paint);

    QPainterPath path;

    paint.setPen(QPen(Qt::blue,1));

    double step = 0.01;
    bool first = true;

    for(double i = xMin; i <= xMax; i+=step){
        if(first){
           path.moveTo((i+oX)*scaleX,(oY-f(i))*scaleY);
           first = false;
        }
        else {
            path.lineTo((i+oX)*scaleX,(oY-f(i))*scaleY);
        }
    }

    paint.drawPath(path);
}

//FIX ME
void MainWindow::drawPoints(QPainter & painter,int count, const QStringList & listX, const QStringList & listY){
    //return;
    //painter.setPen(QPen(Qt::red,3));
    for(int i = 0; i < count; i++){
        double x = (oX+listX[i].toDouble())*scaleX;
        double y = (oY-listY[i].toDouble())*scaleY;

        painter.setPen(QPen(Qt::gray,1));
        QLine l1(x,0,x,h);
        QLine l2(0,y,w,y);

        painter.drawLine(l1);
        painter.drawLine(l2);

        painter.setPen(QPen(Qt::red,3));
        painter.drawPoint(x,y);
    }
}

void MainWindow::solve(int count, const QStringList& listX, const QStringList& listY){
    std::vector<double> y(count), x(count);

    for(int i=0;i<count;i++){
        x[i] = listX[i].toDouble();
        y[i] = listY[i].toDouble();
    }

    std::vector<std::vector<double>>matrix(count, std::vector<double>(count));

    for(int i=0;i<count;i++)
        for(int j=0;j<count;j++)
            matrix[i][j] = pow(x[i],j);


    EquationSystem sys(matrix,y);

    std::vector<double> res;

    if(sys.solve(res) == 0){
        p.setValue(res);

        QPixmap map(w, h);
        QPainter paint;

        paint.begin(&map);

        draw(paint);
        drawPoints(paint,count,listX,listY);

        paint.end();

        ui->output->setPixmap(map);

        QString strAns = getStrAns(res);
        ui->outputLnX->setText(strAns);
    }
}


void MainWindow::on_btn_Solve2_clicked(){
    int count = ui->sb_countEqutations->value();
    QString dataY = ui->le_value->text();
    QString dataX = ui->le_key->text();

    QStringList listX = dataX.split(',');
    QStringList listY = dataY.split(',');

    solve(count, listX, listY);

    QStandardItemModel* model = new QStandardItemModel;
    QStandardItem* item;

    for(int i = 0; i < count; i++){
        item = new QStandardItem("F(" + QString(listX[i]) + ") = " + QString(listY[i]));
        model->appendRow(item);
    }

    ui->lw_Sample->setModel(model);
}
