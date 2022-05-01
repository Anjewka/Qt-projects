//#include "menu.h"
//#include "fractal.h"

//Menu::Menu()
//{
//    btn = new QPushButton(this);

//    lineEdit_list = new std::list<QLineEdit*>;
//    lineEdit_list->resize(6);
//    for(auto &i : *lineEdit_list) {i = new QLineEdit(this);}

//    label_list = new std::list<QLabel*>;
//    label_list->resize(10);
//    for(auto &i : *label_list) {i = new QLabel(this);}

//    radio_list = new std::list<QRadioButton*>;
//    radio_list->resize(3);

//    for(auto &i : *radio_list) {i = new QRadioButton(this);}
//}

//void Menu::setMenu(const double &Re, const double &Im, const double& max_iterations, const double& size, const double& x0, const double& y0)
//{
//    btn->connect(btn, &QPushButton::clicked, &fractal::Start);

//    std::list<QRadioButton*>::iterator i = radio_list->begin();
//    (*i)->setChecked(true);
//    (*i)->connect(*i, &QRadioButton::clicked, &fractal::SwapRadioButton); ++i;
//    (*i)->connect(*i, &QRadioButton::clicked, &fractal::SwapRadioButton); ++i;
//    (*i)->connect(*i, &QRadioButton::clicked, &fractal::SwapRadioButton); ++i;

//    std::list<QLineEdit*>::iterator j = lineEdit_list->begin();
//    (*j)->setText(QString::number(Re)); ++j;
//    (*j)->setText(QString::number(Im)); ++j;
//    (*j)->setText(QString::number(max_iterations)); ++j;
//    (*j)->setText(QString::number(size)); ++j;
//    (*j)->setText(QString::number(x0)); ++j;
//    (*j)->setText(QString::number(y0)); ++j;

//    j = lineEdit_list->begin();
//    (*j)->connect(*j, &QLineEdit::textChanged, this, &Menu::TextChanged); ++j;
//    (*j)->connect(*j, &QLineEdit::textChanged, this, &Menu::TextChanged); ++j;
//    (*j)->connect(*j, &QLineEdit::textChanged, this, &Menu::TextChanged); ++j;
//    (*j)->connect(*j, &QLineEdit::textChanged, this, &Menu::TextChanged); ++j;
//    (*j)->connect(*j, &QLineEdit::textChanged, this, &Menu::TextChanged); ++j;
//    (*j)->connect(*j, &QLineEdit::textChanged, this, &Menu::TextChanged); ++j;

//    btn->setVisible(true);
//    btn->setEnabled(true);

//    for(auto &i : *lineEdit_list) {i->setVisible(true); i->setEnabled(true);}
//    for(auto &i : *label_list) {i->setVisible(true); i->setEnabled(true);}
//    for(auto &i : *radio_list) {i->setVisible(true); i->setEnabled(true);}
//}

//bool Menu::radio_active(int k)
//{
//    return (*radio_list->begin() + k)->isChecked();
//}

//void Menu::hide()
//{
//    btn->setVisible(false);
//    btn->setEnabled(false);

//    for(auto &i : *lineEdit_list) {i->setVisible(false); i->setEnabled(false);}
//    for(auto &i : *label_list) {i->setVisible(false); i->setEnabled(false);}
//    for(auto &i : *radio_list) {i->setVisible(false); i->setEnabled(false);}
//}

//void Menu::show()
//{
//    btn->setVisible(true);
//    btn->setEnabled(true);

//    for(auto &i : *lineEdit_list) {i->setVisible(true); i->setEnabled(true);}
//    for(auto &i : *label_list) {i->setVisible(true); i->setEnabled(true);}
//    for(auto &i : *radio_list) {i->setVisible(true); i->setEnabled(true);}
//}

//bool Menu::TextChanged(double &Re, double &Im, double& max_iterations, double& size, double& x0, double& y0)
//{
//    std::list<QLineEdit*>::iterator i = lineEdit_list->begin();
//    QString s1 = (*i)->text(); ++i;
//    QString s2 = (*i)->text(); ++i;
//    QString s3 = (*i)->text(); ++i;
//    QString s4 = (*i)->text(); ++i;
//    QString s5 = (*i)->text(); ++i;
//    QString s6 = (*i)->text(); ++i;

//    bool c1; bool c2; bool c3; bool c4; bool c5; bool c6; bool c7 = true;
//    double a = s1.toDouble(&c1);
//    double b = s2.toDouble(&c2);
//    double c = s3.toDouble(&c3);
//    double d = s4.toDouble(&c4);
//    double e = s5.toDouble(&c5);
//    double f = s6.toDouble(&c6);

//    std::vector<QPalette> p(lineEdit_list->size());

//    if(c1 && a >= -2 && a <= 2)
//    {
//        p[0].setColor(QPalette::Text, QColor(0, 169, 28));
//    }
//    else {p[0].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

//    if(c2 && b >= -2 && b <= 2)
//    {
//        p[1].setColor(QPalette::Text, QColor(0, 169, 28));
//    }
//    else {p[1].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

//    if(c3 && c > 0)
//    {
//        p[2].setColor(QPalette::Text, QColor(0, 169, 28));
//    }
//    else {p[2].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

//    if(c4 && d > 0)
//    {
//        p[3].setColor(QPalette::Text, QColor(0, 169, 28));
//    }
//    else {p[3].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

//    if(c5 && e < 10000 && c > -10000)
//    {
//        p[4].setColor(QPalette::Text, QColor(0, 169, 28));
//    }
//    else {p[4].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

//    if(c6 && f < 10000 && f > -10000)
//    {
//        p[5].setColor(QPalette::Text, QColor(0, 169, 28));
//    }
//    else {p[5].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

//    int j = 0;
//    for(auto &i : *lineEdit_list) {i->setPalette(p[j]); ++j;}

//    if(c7) {Re = a, Im = b; max_iterations = c; size = d; x0 = e; y0 = f;}
//    return c7;
//}

//void Menu::DrawMenu(double& width, double& height, fractal::Color& color)
//{
//    QFont font1("Bad Script", 15);
//    QFont font2("Bad Script", 10);
//    QFont font3("Agency FB", 15);
//    font1.setPointSize(15);

//    btn->setGeometry((width - 290) / 2, height * 0.3 + 230, 290, 50);

//    std::list<QLabel*>::iterator i = label_list->begin();
//    (*i)->setGeometry((width - 290) / 2, height * 0.3, 50, 30); ++i;
//    (*i)->setGeometry((width - 290) / 2 + 150, height * 0.3, 50, 30); ++i;
//    (*i)->setGeometry((width - 290) / 2 + 280, height * 0.3, 50, 30); ++i;
//    (*i)->setGeometry((width - 290) / 2, height * 0.3 + 30, 130, 50); ++i;
//    (*i)->setGeometry((width - 290) / 2, height * 0.3 + 70, 70, 50); ++i;
//    (*i)->setGeometry((width - 290) / 2, height * 0.3 + 110, 30, 50); ++i;
//    (*i)->setGeometry((width - 290) / 2 + 155, height * 0.3 + 110, 30, 50); ++i;
//    (*i)->setGeometry((width - 290) / 2 + 40, height * 0.3 + 170, 70, 20); ++i;
//    (*i)->setGeometry((width - 290) / 2 + 105, height * 0.3 + 170, 70, 20); ++i;
//    (*i)->setGeometry((width - 290) / 2 + 190, height * 0.3 + 170, 70, 20); ++i;

//    for(auto &i : *label_list) {i->setFont(font1);}
//    i = label_list->end(); --i;
//    (*i)->setFont(font2); --i;
//    (*i)->setFont(font2); --i;
//    (*i)->setFont(font2); --i;

//    switch (color)
//    {
//    case fractal::colorfull:
//    {
//        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : white; }");}
//        break;
//    }
//    case fractal::black_white:
//    {
//        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : white; }");}
//        break;
//    }
//    case fractal::smooth:
//    {
//        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : black; }");}
//        break;
//    }
//    }

//    i = label_list->begin();
//    (*i)->setText("C = ("); ++i;
//    (*i)->setText("+ i"); ++i;
//    (*i)->setText(")"); ++i;
//    (*i)->setText("Max iterations = "); ++i;
//    (*i)->setText("Scope = "); ++i;
//    (*i)->setText("X = "); ++i;
//    (*i)->setText("Y = "); ++i;
//    (*i)->setText("Colorfull"); ++i;
//    (*i)->setText("Black-white"); ++i;
//    (*i)->setText("Smooth"); ++i;

//    for(auto &i : *lineEdit_list) {i->setFont(font3);}

//    std::list<QLineEdit*>::iterator j = lineEdit_list->begin();
//    (*j)->setGeometry((width - 290) / 2 + 50, height * 0.3, 100, 30); ++j;
//    (*j)->setGeometry((width - 290) / 2 + 180, height * 0.3, 100, 30); ++j;
//    (*j)->setGeometry((width - 290) / 2 + 130, height * 0.3 + 40, 160, 30); ++j;
//    (*j)->setGeometry((width - 290) / 2 + 65, height * 0.3 + 80, 225, 30); ++j;
//    (*j)->setGeometry((width - 290) / 2 + 30, height * 0.3 + 120, 105, 30); ++j;
//    (*j)->setGeometry((width - 290) / 2 + 185, height * 0.3 + 120, 105, 30); ++j;

//    btn->setStyleSheet("QPushButton { background-color: white; }");
//    btn->setFont(font1);
//    btn->setText("Start");

//    std::list<QRadioButton*>::iterator k = radio_list->begin();
//    (*k)->setGeometry((width - 290) / 2 + 55, height * 0.3 + 190, 20, 20); ++k;
//    (*k)->setGeometry((width - 290) / 2 + 130, height * 0.3 + 190, 20, 20); ++k;
//    (*k)->setGeometry((width - 290) / 2 + 205, height * 0.3 + 190, 20, 20); ++k;
//}
