#ifndef FIGURE_H
#define FIGURE_H

#include <QVector>
#include <QColor>
#include <position.h>

class figure
{
private:
    int form;
    QVector<position> blocks;
    position* center;
    QColor color;

public:
    figure();

    virtual int getForm();
    virtual QVector<position> getBlocks();
    virtual position* getC();
    virtual QColor getColor();
    virtual void setShape();
    virtual void setBlocks(QVector<position> b);
    virtual void setC(position c1);
    virtual void setColor(QColor c);
    virtual void setCx(int x1);
    virtual void setCy(int y1);

    void Tern();
};

#endif // FIGURE_H
