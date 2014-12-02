#ifndef NEATSTRUCTURE_H
#define NEATSTRUCTURE_H

#include "neatNode.h"
#include "neatConnection.h"
#include "geneticEncoding.h"
#include "neatLayer.h"

#include <QWidget>

namespace Ui {
class NeatStructure;
}

class NeatStructure : public QWidget
{
    Q_OBJECT

public:
    explicit NeatStructure(QWidget *parent = 0);
    ~NeatStructure();

    // Updates all the UIs (layers, nodes, connections)
    // using the information from GeneticEncoding
    void updateUI();

public slots:

    // GeneticEncoding functions with some extra functionalities
    void autogenerateGeneticEncoding();
    void newNode();
    void newConnection();
    void deleteSelected();
    void layerChanged();
    void saveFile();
    void loadFile();
    void setCPPN(bool check);

    // When a parameter of the Node UI is changed
    void updateNodeLayer(int id);
    void updateNodeFunction(int id);
    void updateNodeType(int id);

    // When a parameter of the Connection UI is changed
    void updateConnectionIn(int inn);
    void updateConnectionOut(int inn);
    void updateConnectionWeight(int inn);
    void updateConnectionEnable(int inn);

private:

    // Graphic stuff
    Ui::NeatStructure *ui;

    // Main data structure where all the informatoin is stored
    GeneticEncoding * geneticEncoding;

    // Misc
    NeatLayer * getLayerWidget();
    NeatNode * getNodeWidgetAt(int index);
    NeatConnection * getConnectionWidgetAt(int index);
    int getNodeLayoutCount();
    int getConnectionLayoutCount();

};

#endif // NEATSTRUCTURE_H
