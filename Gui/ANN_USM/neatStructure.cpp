#include "neatStructure.h"
#include "ui_neatStructure.h"

#include <QInputDialog>
#include <QMessageBox>

/**
 * @brief NeatStructure::NeatStructure This widget class contains all the widgets and their connections
 * related with the NEAT structure. i.e. the layers, the nodes and the conections.
 * @param parent
 */
NeatStructure::NeatStructure(QWidget *parent) : QWidget(parent), ui(new Ui::NeatStructure) {
    ui->setupUi(this);

    // Objects
    // -------

    // Default values of input and output nodes, in order to autogenerate the first NEAT draft
    int inputNum = 2;
    int outputNum = 1;

    // Updates the UI spins
    ui->InSpinBox->setValue(inputNum);
    ui->OutSpinBox->setValue(outputNum);

    // Creates the layer widget (this will be unic along the application)
    ui->NEATLayerVerticalLayout->addWidget( new NeatLayer(this) );

    // Creates the main data structure of genetic encoding
    // and autogenerate the first structure
    geneticEncoding = new GeneticEncoding(this);
    autogenerateGeneticEncoding(); // This function calls UpdateUI();

    // Connections
    // -----------

    // File management buttons
    connect(ui->SavePushButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(ui->LoadPushButton, SIGNAL(clicked()), this, SLOT(loadFile()));

    // Node/Connection management buttons
    connect(ui->NewNodePushButton, SIGNAL(clicked()), this, SLOT(newNode()));
    connect(ui->NewConnectionPushButton, SIGNAL(clicked()), this, SLOT(newConnection()));
    connect(ui->DeletePushButton, SIGNAL(clicked()), this, SLOT(deleteSelected()));

    // Autogenerate button
    connect(ui->GeneratePushButton, SIGNAL(clicked()), this, SLOT(autogenerateGeneticEncoding()));

    // Layer changed
    connect(getLayerWidget(), SIGNAL(layerChanged()), this, SLOT(layerChanged()));

    // CPPN checkbox
    connect(ui->CPPNCheckBox, SIGNAL(clicked(bool)), this, SLOT(setCPPN(bool)));
}
NeatStructure::~NeatStructure() {
    delete ui;
    delete geneticEncoding;
}
/**
 * @brief NeatStructure::updateUI This class update the UI elements
 * using ONLY the data from geneticEncoding
 */
void NeatStructure::updateUI() {

    // Updates the layers
    // ------------------

    getLayerWidget()->setLayers(geneticEncoding->getLayers());

    // Delete current node & connection widgets
    // ----------------------------------------

    QLayoutItem *child;

    // Delete nodes
    while ((child = ui->NEATNodeVerticalLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    // Delete coonnections
    while ((child = ui->NEATConnectionVerticalLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    // Create the new node & connection widgets
    // ----------------------------------------

    // Creates nodes
    NeatNode * node;
    for( int i = 0; i < geneticEncoding->getNodeSize(); i++) {

        // create the node
        node = new NeatNode(i,this);
        node->setLayers(geneticEncoding->getLayers());
        node->setLayer(geneticEncoding->getNodeLayer(i));
        node->setType(geneticEncoding->getNodeType(i));
        node->setFunctions(geneticEncoding->getStrFunctions());
        node->setFunction(geneticEncoding->getNodeFunction(i));

        // Check if node not exists
        if(!geneticEncoding->existNode(i)) node->disable();

        // Append nodeto layout
        ui->NEATNodeVerticalLayout->addWidget(node);

        // Create element connections
        connect(node, SIGNAL(layerChange(int)), this, SLOT(updateNodeLayer(int)));
        connect(node, SIGNAL(functionChange(int)), this, SLOT(updateNodeFunction(int)));
        connect(node, SIGNAL(typeChanged(int)), this, SLOT(updateNodeType(int)));
    }

    // Create connections
    NeatConnection * connection;
    for( int i = 0; i < geneticEncoding->getConnectionSize(); i++ ) {

        // Create the connection
        connection = new NeatConnection(i,this);
        connection->setNodes(ui->NEATNodeVerticalLayout->count());
        connection->setIn(geneticEncoding->getConnectionIn(i));
        connection->setOut(geneticEncoding->getConnectionOut(i));
        connection->setWeight(geneticEncoding->getConnectionWeight(i));
        connection->setEnable(geneticEncoding->getConnectionEnable(i));

        // Check if connection not exists
        if(!geneticEncoding->existConnection(i)) connection->disable();

        // Append connections to layout
        ui->NEATConnectionVerticalLayout->addWidget(connection);

        // Create element connections
        connect(connection, SIGNAL(inChanged(int)), this, SLOT(updateConnectionIn(int)));
        connect(connection, SIGNAL(outChanged(int)), this, SLOT(updateConnectionOut(int)));
        connect(connection, SIGNAL(weightChanged(int)), this, SLOT(updateConnectionWeight(int)));
        connect(connection, SIGNAL(enableChanged(int)), this, SLOT(updateConnectionEnable(int)));
    }
}

// Misc
// ----

NeatLayer * NeatStructure::getLayerWidget() {
    return (NeatLayer*)ui->NEATLayerVerticalLayout->itemAt(0)->widget();
}
NeatNode * NeatStructure::getNodeWidgetAt(int index) {
    return (NeatNode*)ui->NEATNodeVerticalLayout->itemAt(index)->widget();
}
NeatConnection * NeatStructure::getConnectionWidgetAt(int index) {
    return (NeatConnection*)ui->NEATConnectionVerticalLayout->itemAt(index)->widget();
}
int NeatStructure::getNodeLayoutCount() {
    return ui->NEATNodeVerticalLayout->count();
}
int NeatStructure::getConnectionLayoutCount() {
    return ui->NEATConnectionVerticalLayout->count();
}

// General Slots
// -------------

void NeatStructure::autogenerateGeneticEncoding() {
    geneticEncoding->autogenerate(ui->InSpinBox->value(),ui->OutSpinBox->value());
    updateUI();
}
void NeatStructure::deleteSelected() {

    int count;

    // Delete selected nodes
    count = getNodeLayoutCount();
    for( int i = 0; i < count; i++)
        if(getNodeWidgetAt(i)->isChecked())
            geneticEncoding->killNode(i);

    // Delete selected connections
    count = getConnectionLayoutCount();
    for( int i = 0; i < count; i++)
        if(getConnectionWidgetAt(i)->isChecked())
            geneticEncoding->killConnection(i);

    updateUI();
}
void NeatStructure::saveFile() {
    geneticEncoding->saveFile();
    //QMessageBox::information(this, "Ups!", "The node already exists!", QMessageBox::Ok, QMessageBox::Ok);
}
void NeatStructure::loadFile() {
    geneticEncoding->loadFile();
    updateUI();
}
void NeatStructure::setCPPN(bool check) {
    geneticEncoding->setCPPN(check);
    updateUI();
}

// Layer Slots
// -----------

void NeatStructure::layerChanged() {
    geneticEncoding->setLayers(getLayerWidget()->getLayers());
    updateUI();
}

// Nodes Slots
// -----------

void NeatStructure::newNode() {
    bool ok;
    int id = QInputDialog::getInt(this, "New Node","Node historical number:", geneticEncoding->getNodeSize(), 0, 100000, 1, &ok);
    if (ok) {
        if(geneticEncoding->existNode(id)) {
            QMessageBox::information(this, "Ups!", "The node already exists!", QMessageBox::Ok, QMessageBox::Ok);
        }
        else {
            geneticEncoding->addNode(id);
            updateUI();
        }
    }
}
void NeatStructure::updateNodeLayer(int id) {
    geneticEncoding->setNodeLayer(id,getNodeWidgetAt(id)->getLayer());
}
void NeatStructure::updateNodeFunction(int id) {
    geneticEncoding->setNodeFunction(id,getNodeWidgetAt(id)->getFunction());
}
void NeatStructure::updateNodeType(int id) {
    geneticEncoding->setNodeType(id,getNodeWidgetAt(id)->getType());
}

// Conections Slots
// ----------------

void NeatStructure::newConnection() {
    bool ok;
    int inn = QInputDialog::getInt(this, "New Connection","Connection innovation number:", geneticEncoding->getConnectionSize(), 0, 100000, 1, &ok);
    if (ok) {
        if(geneticEncoding->existNode(inn)) {
            QMessageBox::information(this, "Ups!", "The connection already exists!", QMessageBox::Ok, QMessageBox::Ok);
        }
        else {
            geneticEncoding->addConnection(inn);
            updateUI();
        }
    }
}
void NeatStructure::updateConnectionIn(int inn) {
    geneticEncoding->setConnectionIn(inn,getConnectionWidgetAt(inn)->getIn());
}
void NeatStructure::updateConnectionOut(int inn) {
    geneticEncoding->setConnectionOut(inn,getConnectionWidgetAt(inn)->getOut());
}
void NeatStructure::updateConnectionWeight(int inn) {
    geneticEncoding->setConnectionWeight(inn,getConnectionWidgetAt(inn)->getWeight());
}
void NeatStructure::updateConnectionEnable(int inn) {
    geneticEncoding->setConnectionEnable(inn,getConnectionWidgetAt(inn)->getEnable());
}
