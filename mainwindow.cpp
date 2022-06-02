#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <cfloat>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // означает, что будем использовать форму
    a = 1; b = 1; z = 1;
    pictHeight = 370; // задаем высоту картинки
    pictWidth = 540; // и её ширину
    step = 0.1; // задаем начальный шаг
    leftX = -100; rightX = 100; // и начальные значения границ
    leftY = -100; rightY = 100;
    drawGraph(); // сразу же отрисовываем пустой график
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::f(double x, double a, double b, double z)
{
    return a*sin(pow(x,4))+b*cos(pow(x,4))+z;
}
void MainWindow::recountPixels()
{
    onePixelX = 540.0/(rightX-leftX);
    onePixelY = 370.0/(rightY-leftY);
    Ox = fabs(leftX); Oy = rightY;
}
void MainWindow::getData()
{
    // ui->name->method() означает, что мы обращаемся к объекту name, который помещен на форме ui
    leftX = ui->inputLeftX->text().toDouble(); // узнаем границы
    rightX = ui->inputRightX->text().toDouble();
    leftY = ui->inputLeftY->text().toDouble();
    rightY = ui->inputRightY->text().toDouble();
    step = 1.0/ui->inputAccuracy->text().toDouble();// и шаг
    a = ui -> inputA ->text().toDouble();
    b = ui -> InputB ->text().toDouble();
    z = (double)rand() / RAND_MAX;
    ui -> z_out ->setText(QString::number(z));
}
void MainWindow::drawGraph(bool notEmpty)
{
    QPixmap graph(540,370); // создаем саму картинку
    QPainter paint; // и пэинтер
    paint.begin(&graph); // запускаем отрисовку
    paint.eraseRect(0,0,540,370); // очищаем рисунок
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight); // и рисуем координатные оси
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3)); // устанавливаем цвет и толщину "пера"
    for(double i = leftX;i<=rightX;i+=10.0) // рисуем черточки на координатой оси
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = leftY;i<=rightY;i+=10.0)
        paint.drawPoint(Ox*onePixelX,(Oy-i)*onePixelY);

    // если мы не рисуем график, то отображаем координатную ось и выключаемся
    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine)); // снова задаем цвет и тип линии
    paint.setRenderHint(QPainter::Antialiasing, true); // задаем параметры рендеринга
    QPainterPath path,p[3]; // QPainterPath означаем, что мы вначале занесем все необходимые точки, а затем соединим их
    bool first[4] = {1,1,1,1}; // узнаем первая ли точка, или надо сдвигаться

    // последовательно проходимся по всем точкам графика, проверяем, существует ли функция в данной точке, и если существует -- заносим точку в массив отрисовки

    for(double i = (double)leftX+step;i<=(double)rightX;i+=step) {
        if(!std::isnan(f(i,a,b,z))) {
            if(first[0]) {
                path.moveTo((i+Ox)*onePixelX,(Oy-f(i,a,b,z))*onePixelY);
                first[0] = false;
            }
            else
                path.lineTo((i+Ox)*onePixelX,(Oy-f(i,a,b,z))*onePixelY);
        }
    }

    paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
    paint.drawPath(path);
    paint.end(); // заканчиваем рисование
    ui->outputGraph->setPixmap(graph); // и помещаем рисунок на форму
    return;
}
void MainWindow::on_exit_clicked()
{
    this->close(); // при нажатии на кнопку выхода, закрываем окно
}

void MainWindow::on_clear_clicked()
{
    // при нажатии на кнопку очистки, пересчитываем пиксели и рисуем координатную ось
    recountPixels();
    drawGraph();
}

void MainWindow::on_draw_clicked()
{
    // при нажатии на кнопку отрисовки, получаем границы и точность, пересчитываем значение шага и отрисовываем график
getData();
recountPixels();
drawGraph(1);
}
