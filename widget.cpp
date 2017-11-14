#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :    QWidget(parent),    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer;

    carcasa.load(":/Resources/carcasa.png");
    car.load(":/Resources/carcasa.png");
    rotor.load(":/Resources/rotor.png");
    rot.load(":/Resources/rotor.png");

    configLayout();

    connect(ui->pbCalcular, SIGNAL(pressed()),this,SLOT(slot_calcular()));
    connect(ui->sbRPM,SIGNAL(valueChanged(int)),this,SLOT(slot_calcular()));
    connect(ui->leCampoMagnetico,SIGNAL(returnPressed()),this,SLOT(slot_calcular()));
    connect(ui->leDiametro,SIGNAL(returnPressed()),this,SLOT(slot_calcular()));
    connect(ui->leEspiras,SIGNAL(returnPressed()),this,SLOT(slot_calcular()));
    connect(ui->pbInfo,QPushButton::pressed,[&](){
        QString text = "Un alternador funciona por conversión de energía mecánica en eléctrica. Mecánicamente se mueve un imán que genera una fuerza electromotriz inducida, y por lo tanto una corriente eléctrica alterna, en las bobinas. El fenómeno físico que se presenta en este caso, es el de inducción electromagnética (Ley de Faraday) que genera corriente eléctrica sobre una bobina conductora debido a la variación en el tiempo del flujo del campo magnético que la atraviesa.";
        QMessageBox::information(this,"¿Qué está pasando?",text);    });
    connect(ui->pbAcercaDe, QPushButton::pressed, [&](){
        QString text = "Diseñado y desarrollado por Julián Macagno\nFísica III - Ingeniería Informática UBP - 2017";
        QMessageBox::information(this, "Acerca De", text);    });
    connect(timer,QTimer::timeout,[&](){
        angulo -= restar;
        repaint();    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *e)
{ 
    QPainter painter1(this), painter2(this);
    painter1.drawImage(0,0,carcasa);
    painter2.translate(tam/2,tam/2);
    painter2.rotate(angulo);
    painter2.drawImage(-tam/5,-tam/5,rotor.scaled(tam/2.5, tam/2.5));
}

void Widget::resizeEvent(QResizeEvent *e)
{
    if(this->height() < (this->width()*2/3))
        tam = this->height();

    else
        tam = this->width()*2/3;

    carcasa = car.scaled(tam,tam);
    rotor = rot.scaled(tam/2.5,tam/2.5);
}

void Widget::configLayout()
{
    ui->leCampoMagnetico->setValidator(new QDoubleValidator);
    ui->leDiametro->setValidator(new QDoubleValidator);
    ui->leEspiras->setValidator(new QIntValidator);
    this->setTabOrder(ui->leCampoMagnetico, ui->sbRPM);
    this->setTabOrder(ui->sbRPM, ui->leEspiras);
    this->setTabOrder(ui->leEspiras, ui->leDiametro);
    this->setTabOrder(ui->leDiametro, ui->pbCalcular);
    this->setTabOrder(ui->pbCalcular, ui->pbInfo);
}

void Widget::slot_calcular()
{
    float campoMagnetico = ui->leCampoMagnetico->text().toFloat();
    float diametro = ui->leDiametro->text().toFloat();
    float espiras = ui->leEspiras->text().toFloat();
    float rpm = ui->sbRPM->value();
    float fem = campoMagnetico * espiras * rpm * pow(diametro/(1000),2) * 2 * M_PI * 4;

    if(!fem)
        ui->lResultados->setText("Error");

    else
        ui->lResultados->setText(QString::number(fem) + " Voltios maximo. \nFrecuencia " + QString::number(ui->sbRPM->value()/60) + "Hz."); //f = rpm*Paresdepolos / 60

    timer->start(16);
    restar = ((double)ui->sbRPM->text().toInt() / 10);
}
