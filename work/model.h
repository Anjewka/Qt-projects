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

class model : public QWidget
{
private:
    double Width = 30;
    double Height = 30;
    double field_Width = 30;
    double field_Height = 30;
    double W = 30;
    double H = 30;
    double Wb1 = 0;
    double Hb1 = 0;
    double Wb2 = 0;
    double Hb2 = 0;

    int timerId;
    int qsz = 0;
    bool con;
    bool menu;

    QVector<Charge*> q;

    QVector<int> colors;

    QSet<QPair<double , double>> map;

    QPushButton *btnPause;
    QPushButton *btnContinue;
    QPushButton *btnAddnew;
    QPushButton *btnQuit;
    QPushButton *apply;
    QLabel *lbl;
    QLineEdit *number;

public:
    model();

    void setSettings();
    void Move();
    void DrawScene();
    void DrawLines();
    void DrawRightPart();
    void setButton();
    void pressed();
    void Pause();
    void Continue();
    void setRightPart();
    double Fq(Charge *q1, Charge *q2);
    double getDist(Charge *q1, Charge *q2);
    void checkBorder(Charge *q, bool &check1, bool& check2);
    void hideMenu();
    void checkPoints(int i);
    Charge* mergeCharges(Charge *q1, Charge *q2);
    void speedRotate(Charge *q, bool c1, bool c2, double s1, double s2);
    void AddNewQ();
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
};

#endif // MODEL_H
