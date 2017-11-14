#pragma once
#include <QWidget>
#include <QString>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QResizeEvent>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QImage rotor, carcasa, rot, car; //rot y car son las imagenes originales
    QTimer *timer;
    double angulo = 0, restar;
    float tam;
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void configLayout();

private slots:
    void slot_calcular();
};

