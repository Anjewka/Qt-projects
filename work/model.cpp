#include <model.h>
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

    setMenu();
    setEscapeMenu();

    f = 0;
}

void model::setSettings()
{
    timerId = startTimer(1);
    con = true;
    menu = false;

    q.resize(qsz);

    std::srand(static_cast<int>(time(0)));

    for(int i = 0; i<q.size() ; ++i)
    {
        q[i] = new Charge();

        q[i]->getV()->setX(double(rand() % 26) / 20);
        q[i]->getV()->setY(double(rand() % 26) / 20);
        q[i]->setX((rand() % (this->width() - 15)) + 15);
        q[i]->setY((rand() % (this->height() - 15)) + 15);
        q[i]->setM((rand() % 20 + 1) / pow(10, 17));
        q[i]->setQ((rand() % 5) / pow(10, 12));
        if(rand() % 2 == 0) {q[i]->setSign("+");}
        else {q[i]->setSign("-");}
        q[i]->setColor(rand() % 256, rand() % 256, rand() % 256);
    }
}

void model::setMenu()
{
    menu = true;
    apply = new QPushButton(this);
    number = new QLineEdit(this);
    connect(apply, &QPushButton::clicked, this, &model::Start);
}

void model::setEscapeMenu()
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

    btnDelete = new QPushButton(this);
    connect(btnDelete, &QPushButton::clicked, this, &model::DeleteCharge);
    setVisibleEnabled(false, btnDelete);

    btnQuit = new QPushButton(this);
    connect(btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);
    setVisibleEnabled(false, btnQuit);

    btnSwap = new QPushButton(this);
    connect(btnSwap, &QPushButton::clicked, this, &model::swapF);
    setVisibleEnabled(false, btnSwap);

    lbl = new QLabel(this);
    lbl->setVisible(false);
    lbl->setEnabled(false);
}

void model::AddNewQ()
{
    if(q.size() < 11)
    {
        Charge *newq = new Charge();
        newq->getV()->setX(double(rand() % 26) / 20);
        newq->getV()->setY(double(rand() % 26) / 20);
        newq->setX((rand() % (this->width() - 15)) + 15); newq->setY((rand() % (this->height() - 15)) + 15);
        newq->setM((rand() % 20 + 1) / pow(10, 17));
        newq->setSign("-");
        newq->setQ((rand() % 10) / pow(10, 12));

        newq->setColor(rand() % 255, rand() % 255, rand() % 255);
        q.push_back(newq);
    }
}

void model::DeleteCharge()
{
    if(q.size()) {q.pop_back();}
}

void model::Continue()
{
    con = true;
    setVisibleEnabled(false, btnPause);
    setVisibleEnabled(false, btnContinue);
    setVisibleEnabled(false, btnAddnew);
    setVisibleEnabled(false, btnDelete);
    setVisibleEnabled(false, btnQuit);
    setVisibleEnabled(false, btnSwap);

    lbl->setVisible(false);
    lbl->setEnabled(false);
}

void model::DrawMenu()
{
    apply->setText("Apply");
    apply->setGeometry(this->width() * 0.4, this->height() * 0.45, this->width() * 0.2, this->height() * 0.1);

    number->setGeometry(this->width() * 0.4, this->height() * 0.4, this->width() * 0.2, this->height() * 0.05);
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

void model::Move()
{
    updateSpeed();
    for(int i = 0; i<q.size() ; ++i)
    {
        if(q[i]->getVx() > 1) {q[i]->getV()->setX(q[i]->getVx() / 10);}
        if(q[i]->getVy() > 1) {q[i]->getV()->setY(q[i]->getVy() / 10);}

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
    if(check1) {q->setSpeed(s1, -1 * s2);}
    else if(check2) {q->setSpeed(-1 * s1, s2);}
}

void model::Annigilation(Charge *q1, Charge *q2)
{
    QColor t = q1->getColor();
    q1->setColor(q2->getColor());
    q2->setColor(t);

    QString sign = q1->getSign();
    q1->setSign(q2->getSign());
    q2->setSign(sign);
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

void model::swapF()
{
    f++;
    f %= 3;
}

double model::Fq(Charge *q1, Charge *q2)
{
    if(!f) {return (9e9 * q1->getQ() * q2->getQ() / (getDist(q1, q2) * getDist(q1, q2)));}
    else if(f == 1) {return 100000 * q1->getQ() * q2->getQ() / (getDist(q1, q2));}
    return 0;
}

double model::getDist(Charge *q1, Charge *q2)
{
    return sqrt(pow(q1->getX() - q2->getX(), 2) + pow(q1->getY() - q2->getY(), 2));
}

void model::checkBorder(Charge *q, bool& check1, bool& check2)
{
    double x0 = q->getX(), y0 = q->getY(), x1 = q->getV()->getX() + x0, y1 = q->getV()->getY() + y0;
    if(this->width() < x1 + 15 || x1 < 15 || this->height() < y1 + 15 || y1 < 15)
    {
        double x, y;
        if(this->width() < x1 + 15 || x1 < 15)
        {
            x = x1 < 15 ? 15 : this->width() - 15;
            y = (x - x0) / (x1 - x0) * (y1 - y0);
        }
        else
        {
            y = y1 < 15 ? 15 : this->height() - 15;
            x = (y - y0) / (y1 - y0) * (x1 - x0);
        }
        if(this->width() < x1 + 15 || x1 < 15) {q->setSpeed(x - x0, y); check2 = true;}
        else {q->setSpeed(x, y - y0); check1 = true;}
    }
}

void model::checkPoints(int i)
{
    for(int j = i + 1; j<q.size() ; ++j)
    {
        if(getDist(q[i], q[j]) < 30)
        {
            if(q[i]->getSign() != q[j]->getSign() && (q[i]->getM() / q[j]->getM() > 4 || q[j]->getM() / q[i]->getM() > 4))
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
    Charge* q = new Charge();
    q->setM(q1->getM() + q2->getM());
    if(q1->getM() - q2->getM())
    {
        q->setX(q1->getX());
        q->setY(q1->getY());
        q->setSpeed(q1->getV());
    }
    else
    {
        q->setX(q2->getX());
        q->setY(q2->getY());
        q->setSpeed(q2->getV());
    }
    q->setColor(rand() % 255, rand() % 255, rand() % 255);
    return q;
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

    double dt = (30 - r) / (vn1 - vn2);

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

    Annigilation(q1, q2);
}

void model::timerEvent(QTimerEvent *)
{
    this->repaint();

    if(con)
    {
        Move();
    }
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

void model::updateSpeed()
{
    updateAcceleratio();
    for(int i = 0; i<q.size() ; ++i)
    {
        q[i]->setSpeed(q[i]->getV()->getX() + q[i]->getA()->getX(), q[i]->getV()->getY() + q[i]->getA()->getY());
    }
}

void model::DrawScene()
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    for(int i = 0; i < q.size() ; ++i)
    {
        p.setBrush(q[i]->getColor());
        p.drawEllipse(q[i]->getX() - 15, q[i]->getY() - 15, 30, 30);
    }
}

void model::DrawEscapeMenu()
{
    QFont font = btnPause->font();
    font.setPointSize(15);

    btnPause->setFont(font);
    btnPause->setGeometry(0.3 * this->width(), 0.25 * this->height(), 0.4 * this->width(), 0.05 * this->height());
    btnPause->setText("Pause");
    setVisibleEnabled(true, btnPause);

    btnContinue->setFont(font);
    btnContinue->setGeometry(0.3 * this->width(), 0.3 * this->height(), 0.4 * this->width(), 0.05 * this->height());
    btnContinue->setText("Continue");
    setVisibleEnabled(true, btnContinue);

    btnAddnew->setFont(font);
    btnAddnew->setGeometry(0.3 * this->width(), 0.35 * this->height(), 0.4 * this->width(), 0.05 * this->height());
    btnAddnew->setText("Add new Charge");
    setVisibleEnabled(true, btnAddnew);

    btnDelete->setFont(font);
    btnDelete->setGeometry(0.3 * this->width(), 0.4 * this->height(), 0.4 * this->width(), 0.05 * this->height());
    btnDelete->setText("Delete Charge");
    setVisibleEnabled(true, btnDelete);

    btnQuit->setFont(font);
    btnQuit->setGeometry(0.3 * this->width(), 0.45 * this->height(), 0.4 * this->width(), 0.05 * this->height());
    btnQuit->setText("Quit");
    setVisibleEnabled(true, btnQuit);

    btnSwap->setFont(font);
    btnSwap->setGeometry(0.3 * this->width(), 0.5 * this->height(), 0.4 * this->width() - 2, 0.05 * this->height());
    btnSwap->setText("Change F");
    setVisibleEnabled(true, btnSwap);

    lbl->setText("Menu");
    lbl->setVisible(true);
    lbl->setEnabled(true);

    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    lbl->setPalette(pal);

    lbl->setGeometry(0.45 * this->width(), 0.2 * this->height(), 0.2 * this->width(), 0.05 * this->height());
    lbl->setStyleSheet("color: rgb(0, 0, 0)");

    QFont f = lbl->font();
    f.setPointSize(20);
    lbl->setFont(f);
}

void model::setVisibleEnabled(bool c, QPushButton *q)
{
    q->setVisible(c);
    q->setEnabled(c);
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
        if(!con)
        {
            DrawEscapeMenu();
        }
    }
}

void model::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Escape:
    {
        if(con) {Pause();}
        else {Continue();}
        break;
    }
    case Qt::Key_Plus:
    {
        AddNewQ();
        break;
    }
    case Qt::Key_Minus:
    {
        DeleteCharge();
        break;
    }
    case Qt::Key_F:
    {
        swapF();
        break;
    }
    }
}
