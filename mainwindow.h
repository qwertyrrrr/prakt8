#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph(bool notEmpty = 0); // функция, отвечающая за отрисовку графика
    void recountPixels(); // в зависимости от точности (об этом позже) считаем количество пикселей на один условный сантиметр
    void getData(); // получаем значения границ графика и точности
    double f(double x, double a, double b, double z);

private slots:
    void on_exit_clicked(); // что будет если кликнуть на кнопку выхода

    void on_clear_clicked(); // ... на кнопку очистки

    void on_draw_clicked(); // ... на кнопку отрисовки

private:
    Ui::MainWindow *ui;
    double leftX,rightX; // границы по х
    double leftY,rightY; // границы по у
    double a, b, z;
    int pictWidth,pictHeight; // ширина и высота картинки
    double step; // шаг (точность)
    double onePixelX,onePixelY; // количество пикселей на шаг
    double Ox,Oy; // координаты центра
};
#endif // MAINWINDOW_H
