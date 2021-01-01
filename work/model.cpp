#include "model.h"
#include <QPalette>
#include <QTime>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QPushButton>
#include <QApplication>
#include <QPair>
#include <ctime>
#include <random>
#include <cmath>

model::model()
{
    resize(900, 900);

    this->setMinimumHeight(400); this->setMinimumWidth(400);

    this->setWindowTitle("Model");

    setRightPart();
    setMenu();

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);
}

void model::setSettings()
{

    timerId = startTimer(0);
    con = true;
    menu = false;

    q.resize(qsz);

    std::srand(static_cast<int>(time(0)));

    for(int i = 0; i<q.size() ; ++i)
    {
        q[i] = new Charge();

        q[i]->getV()->setX(double(rand() % 26) * 0.1);
        q[i]->getV()->setY(double(rand() % 26) * 0.1);
        q[i]->setX(double(rand() % int(field_Width)));
        q[i]->setY(double(rand() % int(field_Height)));
        q[i]->setM((rand() % 10) / pow(10, 18));
        q[i]->setQ((rand() % 5) / pow(10, 15));
        if(rand() % 2 == 0) {q[i]->setSign("+");}
        else {q[i]->setSign("-");}
        q[i]->setColor(rand() % 256, rand() % 256, rand() % 256);
    }
}

void model::DrawScene()
{
    QPainter p(this);

    for(int i = 0; i < q.size() ; ++i)
    {
        p.setBrush(q[i]->getColor());
        p.drawEllipse(q[i]->getX() * Width, q[i]->getY() * Height, Width, Height);
    }
}

void model::DrawLines()
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::black, 2, Qt::SolidLine);
    p.setPen(pen);
    p.drawLine(W + 1, 0, W + 1, H);
}

void model::DrawRightPart()
{
    QFont font = btnPause->font();
    font.setPointSize(15);

    btnPause->setFont(font);
    btnPause->setGeometry(0.8*this->width() + 2, 0.15*this->height(), 0.2*this->width() - 2, 0.05*this->height());
    btnPause->setText("Pause");
    setVisibleEnabled(true, btnPause);

    btnContinue->setFont(font);
    btnContinue->setGeometry(0.8*this->width() + 2, 0.2*this->height(), 0.2*this->width() - 2, 0.05*this->height());
    btnContinue->setText("Continue");
    setVisibleEnabled(true, btnContinue);

    btnAddnew->setFont(font);
    btnAddnew->setGeometry(0.8*this->width() + 2, 0.25*this->height(), 0.2*this->width() - 2, 0.05*this->height());
    btnAddnew->setText("Add new Charge");
    setVisibleEnabled(true, btnAddnew);

    btnQuit->setFont(font);
    btnQuit->setGeometry(0.8*this->width() + 2, 0.3*this->height(), 0.2*this->width() - 2, 0.05*this->height());
    btnQuit->setText("Quit");
    setVisibleEnabled(true, btnQuit);

    lbl->setText("Buttons");
    lbl->setVisible(true);
    lbl->setEnabled(true);

    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    lbl->setPalette(pal);

    lbl->setGeometry(0.85*this->width() + 2, 0.1*this->height(), 0.2*this->width() - 2, 0.05*this->height());
    lbl->setStyleSheet("color: rgb(0, 0, 0)");

    QFont f = lbl->font();
    f.setPointSize(20);
    lbl->setFont(f);
}

void model::DrawMenu()
{
    apply->setText("Apply");
    apply->setGeometry(this->width() * 0.4, this->height() * 0.45, this->width() * 0.2, this->height() * 0.1);

    number->setGeometry(this->width() * 0.4, this->height() * 0.4, this->width() * 0.2, this->height() * 0.05);
    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    number->setPalette(p);

}

void model::Start()
{
    QString s = number->text();
    bool c;
    int a = s.toInt(&c);
    if(a > 0 && a < 6)
    {
        menu = false;
        con = true;
        apply->setVisible(false);
        apply->setEnabled(false);

        number->setVisible(false);
        number->setEnabled(false);
        setSettings();
    }
}

void model::Pause()
{
    con = false;
}

void model::Continue()
{
    con = true;
}

void model::setRightPart()
{
    btnPause = new QPushButton(this);
    connect(btnPause, &QPushButton::clicked, this, &model::Pause);
    setVisibleEnabled(false, btnPause);

    btnContinue = new QPushButton(this);
    connect(btnContinue, &QPushButton::clicked, this, &model::Continue);
    setVisibleEnabled(false, btnContinue);

    btnAddnew = new QPushButton(this);
    connect(btnAddnew, &QPushButton::clicked, this, &model::AddNewQ);
    setVisibleEnabled(false, btnAddnew);

    btnQuit = new QPushButton(this);
    connect(btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);
    setVisibleEnabled(false, btnQuit);

    lbl = new QLabel(this);
    lbl->setVisible(false);
    lbl->setEnabled(false);
}

void model::Move()
{
    updateSpeed();
    for(int i = 0; i<q.size() ; ++i)
    {
        if(q[i]->getVx() - 0.05 >= 0) {q[i]->setSpeed(0.01, q[i]->getVy());}
        if(q[i]->getVy() - 0.05 >= 0) {q[i]->setSpeed(q[i]->getVx(), 0.01);}
        bool check1 = false; bool check2 = false;
        double s1 = q[i]->getV()->getX(); double s2 = q[i]->getV()->getY();

        checkBorder(q[i], check1, check2);
        checkPoints(i);

        q[i]->setX(q[i]->getX() + q[i]->getV()->getX());
        q[i]->setY(q[i]->getY() + q[i]->getV()->getY());

        if(i < q.size()) speedRotate(q[i], check1, check2, s1, s2);
    }
}

void model::speedRotate(Charge *q, bool check1, bool check2, double s1, double s2)
{
    if(check1) {q->setSpeed(s1, -1*s2);}
    else if(check2) {q->setSpeed(-1*s1, s2);}
}

void model::updateAcceleratio()
{
    for(int i = 0; i<q.size() ; ++i)
    {
        acceleratio *a = new acceleratio(0, 0);
        for(int j = 0; j<q.size() ; ++j)
        {
            if(i != j)
            {
                double value;
                if(q[i]->getM()) {value = Fq(q[i], q[j]) / q[i]->getM();}
                else {value = 0;}
                acceleratio *b = new acceleratio(value/getDist(q[i], q[j])*(q[j]->getX() - q[i]->getX()), value/getDist(q[i], q[j])*(q[j]->getY() - q[i]->getY()));
                if(q[i]->getSign() == q[j]->getSign())
                {
                    a->setX(a->getX() + -1*b->getX());
                    a->setY(a->getY() + -1*b->getY());
                }
                else
                {
                    a->setX(a->getX() + b->getX());
                    a->setY(a->getY() + b->getY());
                }
            }
        }
        q[i]->setA(a);
    }
}

double model::Fq(Charge *q1, Charge *q2)
{
    return (9e9 * q1->getQ() * q2->getQ() / (getDist(q1, q2) * getDist(q1, q2)));
}

double model::getDist(Charge *q1, Charge *q2)
{
    return sqrt(pow(q1->getX() - q2->getX(), 2) + pow(q1->getY() - q2->getY(), 2));
}

void model::checkBorder(Charge *q, bool& check1, bool& check2)
{
    double x0 = q->getX(), y0 = q->getY(), x1 = q->getV()->getX() + x0, y1 = q->getV()->getY() + y0;
    if(field_Width - x1 - 1 < 0 || x1 < 0 || field_Height - y1 - 1 < 0 || y1 < 0)
    {
        double x, y;
        if(field_Width - x1 - 1 < 0 || x1 < 0)
        {
            x = x1 < 0 ? 0 : field_Width - 1;
            y = (x - x0) / (x1 - x0) * (y1 - y0);
        }
        else
        {
            y = y1 < 0 ? 0 : field_Height - 1;
            x = (y - y0) / (y1 - y0) * (x1 - x0);
        }
        if(field_Width - x1 - 1 < 0 || x1 < 0) {q->setSpeed(x - x0, y); check2 = true;}
        else {q->setSpeed(x, y - y0); check1 = true;}
    }
}

void model::checkPoints(int i)
{
    for(int j = i + 1; j<q.size() ; ++j)
    {
        if(getDist(q[i], q[j]) <= 1)
        {
            if(q[i]->getSign() != q[j]->getSign() && (Fq(q[i], q[j]) > q[i]->getM() * 0.1 || Fq(q[i], q[j]) > q[j]->getM() * 0.1))
            {
                q[i] = mergeCharges(q[i], q[j]);
                q.erase(q.begin() + j);
            }
            else {Puls(q[i], q[j]);}
        }
    }
}

Charge* model::mergeCharges(Charge *q1, Charge *q2)
{
    Charge *new_q = q1;
    new_q->setColor(QColor(q1->getColor().red() + q2->getColor().red(), q1->getColor().green() + q2->getColor().green(), q1->getColor().blue() + q2->getColor().blue()));
    new_q->setM(q1->getM() + q2->getM()); new_q->setQ(q1->getQ() + q2->getQ());
    if(new_q->getColor() == Qt::white) {new_q->setColor(Qt::black);}
    if(q1->getM() > q2->getM())
    {
        new_q->setX(q1->getX());
        new_q->setY(q1->getY());
        new_q->getV()->setX(q1->getV()->getX());
        new_q->setSign(q1->getSign());
    }
    else
    {
        new_q->setX(q2->getX());
        new_q->setY(q2->getY());
        new_q->setSpeed(q2->getV());
        new_q->setSign(q2->getSign());
    }
    return new_q;
}

void model::AddNewQ()
{
    con = false;
    if(q.size() < 5)
    {
        Charge *newq = new Charge();
        newq->setSpeed((rand() % 20) * 0.01, (rand() % 20) * 0.01);
        newq->setX(rand() % int(field_Width)); newq->setY(rand() % int(field_Height));
        newq->setM((rand() % 10) / pow(10, 5));
        newq->setSign("-");
        newq->setQ((rand() % 10) / pow(10, 18));

        newq->setColor(rand() % 255, rand() % 255, rand() % 255);
        q.push_back(newq);
    }
    con = true;
}

void model::Puls(Charge *q1, Charge *q2)
{
    double r = getDist(q1, q2);

    double cosx = (q2->getX() - q1->getX()) / r;
    double siny = (q2->getY() - q1->getY()) / r;

    double vn1 = q1->getVx() * cosx + q1->getVy() * siny;
    double vt1 = -q1->getVx() * siny + q1->getVy() * cosx;

    double vn2 = q2->getVx() * cosx + q2->getVy() * siny;
    double vt2 = -q2->getVx() * siny + q2->getVy() * cosx;

    double dt = (1 - r) / (vn1 - vn2);

    q1->setX(q1->getX() - q1->getVx() * dt);
    q1->setY(q1->getY() - q1->getVy() * dt);
    q2->setX(q2->getX() - q2->getVx() * dt);
    q2->setY(q2->getY() - q2->getVy() * dt);

    double tmp = vn1;
    vn1 = ((q1->getM() - q2->getM()) * vn1 + 2 * q2->getM() * vn2) / (q1->getM() + q2->getM());
    vn2 = (2 * q1->getM() * tmp + (q2->getM() - q1->getM()) * vn2) / (q1->getM() + q2->getM());

    q1->setSpeed(new Speed(vn1 * cosx - vt1 * siny, vn1 * siny + vt1 * cosx));
    q2->setSpeed(new Speed(vn2 * cosx - vt2 * siny, vn2 * siny + vt2 * cosx));

    q1->setX(q1->getX() + q1->getVx() * dt);
    q1->setY(q1->getY() + q1->getVy() * dt);
    q2->setX(q2->getX() + q2->getVx() * dt);
    q2->setY(q2->getY() + q2->getVy() * dt);
}

void model::getNum()
{
    QString s = number->text();
    bool c;
    int a = s.toInt(&c);
    QPalette p;
    if(a > 0 && a < 6)
    {
        p.setColor(QPalette::Text, Qt::green);
        qsz = a;
    }
    else
    {
        p.setColor(QPalette::Text, Qt::red);
    }
    number->setPalette(p);
}

void model::setMenu()
{
    menu = true;
    apply = new QPushButton(this);
    number = new QLineEdit(this);
    connect(apply, &QPushButton::clicked, this, &model::Start);
}

void model::setVisibleEnabled(bool c, QPushButton *q)
{
    q->setVisible(c);
    q->setEnabled(c);
}

void model::updateScene()
{
    W = this->width() * 0.8;
    H = this->height();

    Width = sqrt(this->width());
    Height = sqrt(this->height());
    field_Width = 0.8 * sqrt(this->width());
    field_Height = Height;

    Wb1 = 0.85 * this->width();
    Hb1 = 0.4 * this->height();
    Wb2 = Wb1 + 0.1 * this->width();
    Hb2 = 0.6 * this->height();
}

void model::updateSpeed()
{
    updateAcceleratio();
    for(int i = 0; i<q.size() ; ++i)
    {
        q[i]->setSpeed(q[i]->getV()->getX() + q[i]->getA()->getX(), q[i]->getV()->getY() + q[i]->getA()->getY());

        if(q[i]->getVx() - 0.05 >= 0) {q[i]->setSpeed(0.01, q[i]->getVy());}
        if(q[i]->getVy() - 0.05 >= 0) {q[i]->setSpeed(q[i]->getVx(), 0.01);}
    }
}

void model::timerEvent(QTimerEvent *)
{
    updateScene();
    this->repaint();

    if(con)
    {
        for(int i = 0; i<q.size() ; ++i)
        {
            if(q[i]->getVx() - 0.05 >= 0) {q[i]->setSpeed(0.01, q[i]->getVy());}
            if(q[i]->getVy() - 0.05 >= 0) {q[i]->setSpeed(q[i]->getVx(), 0.01);}
        }

        Move();
    }
}

void model::paintEvent(QPaintEvent *)
{
    if(menu)
    {
        DrawMenu();
        getNum();
    }
    else
    {
        DrawScene();
        DrawLines();
        DrawRightPart();
    }
}
