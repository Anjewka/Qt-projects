#ifndef MODEL_H
#define MODEL_H

#include <QWidget>
#include <charge.h>
#include <QPushButton>
#include <QLabel>
#include <QPoint>
#include <QSet>
#include <QPair>
#include <QLineEdit>
#include <QKeyEvent>

class model : public QWidget
{
private:
    int timerId;
    int qsz = 0;
    bool con;
    bool menu;
    int f;

    QVector<Charge*> q;

    QPushButton *btnPause;
    QPushButton *btnContinue;
    QPushButton *btnAddnew;
    QPushButton *btnDelete;
    QPushButton *btnSwap;
    QPushButton *btnQuit;
    QPushButton *apply;
    QLabel *lbl;
    QLineEdit *number;

public:
    model();

    void setSettings();
    void Move();
    void DrawScene();
    void DrawEscapeMenu();
    void setButton();
    void pressed();
    void Pause();
    void Continue();
    void setEscapeMenu();
    double Fq(Charge *q1, Charge *q2);
    double getDist(Charge *q1, Charge *q2);
    void checkBorder(Charge *q, bool &check1, bool& check2);
    void hideMenu();
    void checkPoints(int i);
    Charge* mergeCharges(Charge *q1, Charge *q2);
    void speedRotate(Charge *q, bool c1, bool c2, double s1, double s2);
    void checkPositions();
    void Annigilation(Charge *q1, Charge *q2);
    void AddNewQ();
    void swapF();
    void DeleteCharge();
    void Puls(Charge *q1, Charge *q2);
    void getNum();
    void setMenu();
    void updateAcceleratio();
    void Start();
    void Charges();
    void setVisibleEnabled(bool c, QPushButton *q);
    void updateScene();
    void updateSpeed();
    void DrawMenu();

protected:
    void timerEvent(QTimerEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
};

#endif // MODEL_H
