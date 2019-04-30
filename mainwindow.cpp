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
    init(-10, 10);

    xStep = yStep = 1;
    setPoint = false;
    showGrid = ui->cb_showGrid->isChecked();
    onlyInt = ui->cb_onlyInt->isChecked();

    map = new QPixmap(w,h);
    painter = new QPainter(map);

    clearPainter();
    if(showGrid)
        drawGrid();
    drawAxes();

    ui->output->setPixmap(*map);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCount(int c){
    if(c > 7){
        QMessageBox msgBox;
        msgBox.setText("Больше 7 точек использовать нежелательно");
        msgBox.setInformativeText("Продолжить?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDefaultButton(QMessageBox::No);
        int res = msgBox.exec();

        if( res == QMessageBox::Yes){
            count = c;
        }
        else{
            count = 7;
            return;
        }
    }
    else {
        count = c;
    }
}

void MainWindow::init(){
    setX(-15, 15);
    setY(-15, 15);
    setO();
    calcScale();
}

void MainWindow::init(int min, int max){
    setX(min, max);
    setY(min, max);
    setO();
    calcScale();
}

void MainWindow::setX(int min, int max){
    if(min >= max)
        return;
    xMin = min;
    xMax = max;
}

void MainWindow::setY(int min, int max){
    if(min >= max)
        return;
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
    bool wasOne = false;
    QString strAns = "";

    if(!ui->cb_formatOutput->isChecked()){
        for(int i = res.size() - 1;i >= 0;i--){
            if(res[i] < 0)
                strAns += "-";
            else
                strAns += "+";

            strAns += QString::number(fabs(res[i]),'g',2);
            strAns += "x^" + QString::number(i);
        }

        return "L<sub>n</sub>(x) = " + strAns;
    }

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

        if(fabs(res[i] - 1) > eps){
            tmp += QString::number(fabs(res[i]));
        }else{
            wasOne = true;
        }

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

    if(strAns.size() == 0 && wasOne)
        strAns += "1";

    return "L<sub>n</sub>(x) = " + strAns;
}

double MainWindow::f(double x){
    return p(x);
}

void MainWindow::clearPainter(){
    painter->eraseRect(0,0,w,h);
}

void MainWindow::drawGrid(){
    painter->setPen(QPen(Qt::lightGray,1));

    for(int i = xMin + 1; i < xMax; i++){
        QLine l((oX + i) * scaleX,0,(oX + i) * scaleX,h);
        painter->drawLine(l);
    }

    for(int i = yMin + 1; i < yMax; i++){
        QLine l(0,(i + oY) * scaleY,w,(i + oY) * scaleY);
        painter->drawLine(l);
    }

    painter->setPen(QPen(Qt::black,1));
}

void MainWindow::drawAxes(){
    ui->statusBar->showMessage("draw axes start");
    QLine l1(oX * scaleX, 0, oX *scaleX, h);
    QLine l2(0, oY * scaleY, w, oY * scaleY);

    painter->drawLine(l1);
    painter->drawLine(l2);

    painter->setPen(QPen(Qt::black,3));

    for(int i = xMin + 1;i < xMax;i++){
        painter->drawPoint((oX + i)*scaleX, oY * scaleY);
        if(i == 0)
            continue;
        painter->drawText((oX + i)*scaleX - 5, oY * scaleY + 15, QString::number(i));
    }
    //painter.drawText(oX * scaleX - 10, oY * scaleY - 5, "0");
    for(int i = yMin + 1;i < yMax;i++){
        painter->drawPoint(oX * scaleX, (i + oY)*scaleY);
        if(i == 0)
            continue;
        painter->drawText(oX*scaleX + 5, (i + oY)*scaleY + 5, QString::number(-i));
    }
    painter->setPen(QPen(Qt::black,1));
    ui->statusBar->showMessage("draw axes end");
}


void MainWindow::drawGraph(const Polynom & p){
    ui->statusBar->showMessage("draw graph start");
    QPainterPath path;

    painter->setPen(QPen(Qt::blue,1));

    double step = 0.01;
    bool first = true;

    for(double i = xMin; i <= xMax; i+=step){
        if(first){
           path.moveTo((i + oX)*scaleX,(oY - f(i))*scaleY);
           first = false;
        }
        else {
            path.lineTo((i + oX)*scaleX,(oY - f(i))*scaleY);
        }
    }

    painter->drawPath(path);

    painter->setPen(QPen(Qt::black,1));
    ui->statusBar->showMessage("draw graph end");
}

void MainWindow::drawLines(){
    ui->statusBar->showMessage("draw lines start");
    painter->setPen(QPen(Qt::gray,1));

    for(int i = 0; i < count; i++){
        double ptx = (oX + x[i])*scaleX;
        double pty = (oY - y[i])*scaleY;

        QLine l1(ptx, 0, ptx, h);
        QLine l2(0, pty, w, pty);

        painter->drawLine(l1);
        painter->drawLine(l2);
    }

    painter->setPen(QPen(Qt::black,1));
    ui->statusBar->showMessage("draw lines end");
}

void MainWindow::drawPoints(){
    ui->statusBar->showMessage("draw points start");
    painter->setPen(QPen(Qt::red,4));

    for(int i = 0; i < count; i++){
        double ptx = (oX + x[i])*scaleX;
        double pty = (oY - y[i])*scaleY;

        painter->drawPoint(ptx, pty);
    }

    painter->setPen(QPen(Qt::black,1));
    ui->statusBar->showMessage("draw points end");
}

void MainWindow::solve(){
    ui->statusBar->showMessage("solving start");

    std::vector<std::vector<double>>matrix(count, std::vector<double>(count));

    for(int i=0;i<count;i++)
        for(int j=0;j<count;j++)
            matrix[i][j] = pow(x[i],j);


    EquationSystem sys(matrix,y);

    std::vector<double> res;

    if(sys.solve(res) == 0){
        wasSolve = true;

        p.setValue(res);

        clearPainter();
        if(showGrid)
            drawGrid();
        drawAxes();
        drawLines();
        drawGraph(p);
        drawPoints();

        ui->output->setPixmap(*map);

        QString strAns = getStrAns(res);
        ui->outputLnX->setText(strAns);
    }else{
        clearPainter();
        if(showGrid)
            drawGrid();
        drawAxes();
        ui->outputLnX->setText("L<sub>n</sub>(x)");
    }
    ui->statusBar->showMessage("solving end",3000);
}

void MainWindow::setInputData(){
    QStandardItemModel* model = new QStandardItemModel;
    QStandardItem* item;

    for(int i = 0; i < count; i++){
        item = new QStandardItem("F(" + QString::number(x[i]) + ") = " + QString::number(y[i]));
        model->appendRow(item);
    }

    ui->lw_Sample->setModel(model);
}

void MainWindow::on_btn_Solve2_clicked(){
    setCount(ui->sb_countEqutations->value());
    QString dataY = ui->le_value->text();
    QString dataX = ui->le_key->text();

    if(dataX.back()==',')dataX.remove(dataX.size()-1,1);
    if(dataY.back()==',')dataY.remove(dataY.size()-1,1);

    QStringList listX = dataX.split(',');
    QStringList listY = dataY.split(',');

    clearPainter();
    if(showGrid)
        drawGrid();
    drawAxes();

    ui->output->setPixmap(*map);

    if(listX.size() < count){
        QMessageBox::critical(this, "Ошибка ввода", "Недостаточно узлов интерполяции");
        return;
    }

    if(listX.size() > count && listY.size() < count){
        QMessageBox::critical(this, "Ошибка ввода", "Недостаточно значений");
        return;
    }

    x.resize(count);
    y.resize(count);

    for(int i=0;i<count;i++){
        x[i] = listX[i].toDouble();
        y[i] = listY[i].toDouble();
    }

    bool stop = false;

    for(int i = 0; i < count - 1; i++){
        for(int j = i + 1; j < count; j++){
            if(fabs(x[i] - x[j]) < eps){
                stop = true;
            }
        }
    }

    if(stop){
        QMessageBox::critical(this, "Ошибка ввода", "Кратные узлы недопустимы");
        return;
    }

    int maxSz = max(*max_element(all(x)),
                    *max_element(all(y)));

    int minSz = min(*min_element(all(x)),
                    *min_element(all(y)));    

    solve();

   setInputData();
}

void MainWindow::updateGraph(){
    clearPainter();
    if(showGrid)
        drawGrid();
    drawAxes();
    drawLines();
    drawGraph(p);
    drawPoints();
}

void MainWindow::on_btn_chooseFile_clicked(){
    auto path = QFileDialog::getOpenFileName(this,"Выбрать файл","C:\\","*.txt");

    ui->le_FilePath->setText(path);
}

void MainWindow::on_btn_Solve1_clicked()
{
    QString path = ui->le_FilePath->text();

    clearPainter();
    drawAxes();

    ui->output->setPixmap(*map);

    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        ui->statusBar->showMessage("No open file");
        QMessageBox::critical(this, "Ошибка чтения файла", "Файл не найден!");
        return;
    }

    auto str = file.readAll();
    auto data = QString(str);

    auto dataList = data.split('\n');

    setCount(dataList[0].toInt());
    QStringList listX = dataList[1].split(' ');
    QStringList listY = dataList[2].split(' ');

    x.resize(count);
    y.resize(count);

    for(int i =0; i < count; i++){
        x[i] = listX[i].toDouble();
        y[i] = listY[i].toDouble();
    }

    solve();

   setInputData();
}

void MainWindow::on_pb_chX_clicked()
{
    setX(xMin - xStep, xMax + xStep);
    setO();
    calcScale();

    if(wasSolve)
        updateGraph();
    else{
        clearPainter();
        if(showGrid)
            drawGrid();
        drawAxes();
    }

    ui->output->setPixmap(*map);
}

void MainWindow::on_pb_chY_clicked()
{
    setY(yMin - yStep, yMax + yStep);
    setO();
    calcScale();

    if(wasSolve)
        updateGraph();
    else{
        clearPainter();
        if(showGrid)
            drawGrid();
        drawAxes();
    }

    ui->output->setPixmap(*map);
}

void MainWindow::on_pb_chX_minus_clicked()
{
    setX(xMin + xStep, xMax - xStep);
    setO();
    calcScale();

    if(wasSolve)
        updateGraph();
    else{
        clearPainter();
        if(showGrid)
            drawGrid();
        drawAxes();
    }

    ui->output->setPixmap(*map);
}

void MainWindow::on_pb_chY_minus_clicked()
{
    setY(yMin + yStep, yMax - yStep);
    setO();
    calcScale();

    if(wasSolve)
        updateGraph();
    else{
        clearPainter();
        if(showGrid)
            drawGrid();
        drawAxes();
    }

    ui->output->setPixmap(*map);
}

void MainWindow::on_pb_default_clicked()
{
    setY(-10, 10);
    setX(-10, 10);
    setO();
    calcScale();

    clearPainter();
    if(showGrid)
        drawGrid();
    drawAxes();

    wasSolve = false;
    setCount(0);
    x.resize(count);
    y.resize(count);

    ui->lw_Sample->setModel(new QStandardItemModel);

    ui->outputLnX->setText("L<sub>n</sub>(x)");
    ui->output->setPixmap(*map);
}

void MainWindow::mousePressEvent(QMouseEvent *e){
    if(!setPoint)
        return;

    double _x = e->x();
    double _y = e->y();

    auto pixmapPos = ui->output->geometry();

    if((_x >= pixmapPos.x() && _x <= pixmapPos.x() + pixmapPos.width()) &&
            (_y >= pixmapPos.y() && _y <= pixmapPos.y() + pixmapPos.height() )){
        _x = _x - pixmapPos.x();
        _y = _y - pixmapPos.y();
        ui->statusBar->showMessage(QString("%1,%2").arg(_x).arg(_y));
        painter->drawPoint(_x, _y);

        _x = 1.0 * _x / scaleX - oX;
        _y = -1.0 * _y / scaleY + oY;

        if(onlyInt){
            _x = round(_x);
            _y = round(_y);
        }

        for(int i = 0; i < count; i++){
            if(fabs(_x - x[i]) < 0.1 && fabs(_y - y[i]) < 0.1){
                x.erase(x.begin() + i);
                y.erase(y.begin() + i);
                count--;
                solve();
                ui->output->setPixmap(*map);

                setInputData();
                return;
            }
        }

        if(count >= 7){
            QMessageBox::warning(this, "Ошибка ввода", "Больше 7 точек использовать нежелательно");
            return;
        }
        count++;
        x.push_back(_x);
        for(int i = 0; i < count- 1; i++)
            if(fabs(x[i] - x.back()) < eps){
                QMessageBox::critical(this, "Ошибка ввода", "Кратные узлы недопустимы");
                x.pop_back();
                count--;
                return;
            }


        y.push_back(_y);
        solve();

        ui->statusBar->showMessage(QString("%1,%2").arg(_x).arg(_y));
        ui->output->setPixmap(*map);

        setInputData();
    }
    else
        ui->statusBar->showMessage("out pixmap");
}

void MainWindow::on_cb_setPoint_stateChanged(int arg1)
{
    if(arg1 != 0){
        setPoint = true;
        setCount(0);        
        x.resize(count);
        y.resize(count);
        ui->tab->setEnabled(false);
        ui->tab_2->setEnabled(false);
        ui->output->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    }else{
        setPoint = false;
        ui->tab->setEnabled(true);
        ui->tab_2->setEnabled(true);
        ui->output->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    }

    ui->statusBar->showMessage(QString::number(arg1));
}

void MainWindow::on_cb_showGrid_stateChanged(int arg1)
{
    if(arg1 != 0){
        showGrid = true;
    }
    else {
        showGrid = false;
    }
}

void MainWindow::on_cb_onlyInt_stateChanged(int arg1)
{
    if(arg1 != 0){
        onlyInt = true;
    }
    else {
        onlyInt = false;
    }
}
