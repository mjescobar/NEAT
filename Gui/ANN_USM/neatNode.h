#ifndef NEATNODE_H
#define NEATNODE_H

#include <QWidget>
//#include <QSignalMapper>

namespace Ui {
class NeatNode;
}

class NeatNode : public QWidget
{
    Q_OBJECT

public:
    explicit NeatNode(int id, QWidget *parent = 0);
    ~NeatNode();

    // Setters
    void setLayers(QVector< int > layers);
    void setLayer(int layer);
    void setFunction(int function);
    void setFunctions(QStringList functions);
    void setType(int type);

    // Getters
    int getType();
    int getFunction();
    int getLayer();

    // Misc
    void enable();
    void disable();

    bool isChecked();

public slots:

    void sendLayerChange();
    void sendFunctionChange();
    void sendTypeChange();

signals:

    void layerChange(int id);
    void functionChange(int id);
    void typeChanged(int id);

private:

    Ui::NeatNode *ui;

    //QSignalMapper *signalMapper;
};

#endif // NEATNODE_H
