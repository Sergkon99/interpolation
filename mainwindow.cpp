#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    QPixmap map(w, h);
    QPainter paint;

    paint.begin(&map);

    clearPainter(paint);
    drawAxes(paint);

    paint.end();

    ui->output->setPixmap(map);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    setX(-15, 15);
    setY(-15, 15);
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
    bool wasDigit = false;
    QString strAns = "L<sub>n</sub>(x) = ";

    for(int i = res.size() - 1;i >= 0;i--){
        if(fabs(res[i]) < eps)
            continue;

        wasDigit = true;
        QString tmp = "";

        if(firstDigit){
            firstDigit = false;
            if(res[i] < 0)
                tmp += "-";
        }
        else{
            if(res[i] < 0)
                tmp += "-";
            else
                tmp += "+";
        }

        if(fabs(res[i] - 1) > eps)
            tmp += QString::number(fabs(res[i]),'g',2);

        if(i != 0){
            if(i == 1)
                tmp += "x";
            else
                tmp += "x<sup>" + QString::number(i) + "</sup>";
        }
        strAns += tmp;
    }
    if(!wasDigit)
        strAns += "0";

    return strAns;
}

double MainWindow::f(double x){
    return p(x);
}

void MainWindow::clearPainter(QPainter & painter){
    painter.eraseRect(0,0,w,h);
}

void MainWindow::drawAxes(QPainter& painter){   
    ui->statusBar->showMessage("draw axes start");
    QLine l1(oX * scaleX, 0, oX *scaleX, h);
    QLine l2(0, oY * scaleY, w, oY * scaleY);

    painter.drawLine(l1);
    painter.drawLine(l2);

    painter.setPen(QPen(Qt::black,3));

    for(int i = xMin + 1;i < xMax;i++){
        painter.drawPoint((oX + i)*scaleX, oY * scaleY);
        if(i == 0)
            continue;
        painter.drawText((oX + i)*scaleX - 5, oY * scaleY + 15, QString::number(i));
    }
    //painter.drawText(oX * scaleX - 10, oY * scaleY - 5, "0");
    for(int i = yMin + 1;i < yMax;i++){
        painter.drawPoint(oX * scaleX, (i + oY)*scaleY);
        if(i == 0)
            continue;
        painter.drawText(oX*scaleX + 5, (i + oY)*scaleY + 5, QString::number(-i));
    }
    painter.setPen(QPen(Qt::black,1));
    ui->statusBar->showMessage("draw axes end");
}


void MainWindow::drawGraph(QPainter & painter, const Polynom & p){
    ui->statusBar->showMessage("draw graph start");
    QPainterPath path;

    painter.setPen(QPen(Qt::blue,1));

    double step = 0.01;
    bool first = true;

    for(double i = xMin; i <= xMax; i+=step){
        if(first){
           path.moveTo((i + oX)*scaleX,(oY - p(i))*scaleY);
           first = false;
        }
        else {
            path.lineTo((i + oX)*scaleX,(oY - p(i))*scaleY);
        }
    }

    painter.drawPath(path);
    ui->statusBar->showMessage("draw graph end");
}

void MainWindow::drawLines(QPainter & painter,int count, const QStringList & listX, const QStringList & listY){
    ui->statusBar->showMessage("draw lines start");
    painter.setPen(QPen(Qt::gray,1));

    for(int i = 0; i < count; i++){
        double x = (oX + listX[i].toDouble())*scaleX;
        double y = (oY - listY[i].toDouble())*scaleY;

        QLine l1(x, 0, x, h);
        QLine l2(0, y, w, y);

        painter.drawLine(l1);
        painter.drawLine(l2);
    }
    ui->statusBar->showMessage("draw lines end");
}

void MainWindow::drawPoints(QPainter & painter,int count, const QStringList & listX, const QStringList & listY){
    ui->statusBar->showMessage("draw points start");
    painter.setPen(QPen(Qt::red,4));

    for(int i = 0; i < count; i++){
        double x = (oX + listX[i].toDouble())*scaleX;
        double y = (oY - listY[i].toDouble())*scaleY;

        painter.drawPoint(x, y);
    }
    ui->statusBar->showMessage("draw points end");
}

void MainWindow::solve(int count, const QStringList& listX, const QStringList& listY){
    ui->statusBar->showMessage("solving start");
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

        clearPainter(paint);
        drawAxes(paint);
        drawLines(paint, count, listX, listY);
        drawGraph(paint, p);
        drawPoints(paint, count, listX, listY);

        paint.end();

        ui->output->setPixmap(map);

        QString strAns = getStrAns(res);
        ui->outputLnX->setText(strAns);
    }
    ui->statusBar->showMessage("solving end");
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
