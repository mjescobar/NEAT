#ifndef NEATCONNECTION_H
#define NEATCONNECTION_H

#include <QWidget>
//#include <QSignalMapper>

namespace Ui {
class NeatConnection;
}

class NeatConnection : public QWidget
{
    Q_OBJECT

public:
    explicit NeatConnection(int inn, QWidget *parent = 0);
    ~NeatConnection();

    // Setters
    void setNodes(int  nodes);
    void setIn(int in);
    void setOut(int out);
    void setWeight(double weight);
    void setEnable(int enable);

    // Getters
    int getIn();
    int getOut();
    double getWeight();
    int getEnable();

    // Misc
    void enable();
    void disable();

    bool isChecked();

public slots:

    void sendInChange();
    void sendOutChange();
    void sendWeightChange();
    void sendEnableChange();

signals:

    void inChanged(int inn);
    void outChanged(int inn);
    void weightChanged(int inn);
    void enableChanged(int inn);

private:

    Ui::NeatConnection *ui;

    //QSignalMapper *signalMapper;
};

#endif // NEATCONNECTION_H
