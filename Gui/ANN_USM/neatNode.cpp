#include "neatNode.h"
#include "ui_neatNode.h"

NeatNode::NeatNode(int id, QWidget *parent) : QWidget(parent), ui(new Ui::NeatNode) {
    ui->setupUi(this);
    ui->NodeLabel->setText(QString::number(id));

    connect(ui->NodeLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendLayerChange()));
    connect(ui->NodeFunctionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendFunctionChange()));
    connect(ui->NodeTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sendTypeChange()));
}
NeatNode::~NeatNode() {
    delete ui;
}
void NeatNode::enable() {
    ui->NodeCheckBox->setDisabled(false);
    //ui->NodeLabel->setText();
    ui->NodeTypeComboBox->setDisabled(false);
    ui->NodeLayerComboBox->setDisabled(false);
    ui->NodeFunctionComboBox->setDisabled(false);
}
void NeatNode::disable() {
    ui->NodeCheckBox->setDisabled(true);
    //ui->NodeLabel->setText();
    ui->NodeTypeComboBox->setDisabled(true);
    ui->NodeLayerComboBox->setDisabled(true);
    ui->NodeFunctionComboBox->setDisabled(true);
}

// Setters
// -------

// Set all of the possible layers
void NeatNode::setLayers(QVector< int > layers) {
    for( int i = 0; i < layers.size(); i++ )
        ui->NodeLayerComboBox->addItem(QString::number(layers.at(i)));
}
void NeatNode::setLayer(int layer) {
    for( int i = 0; i < ui->NodeLayerComboBox->count(); i++ )
        if(ui->NodeLayerComboBox->itemText(i) == QString::number(layer)) {
            ui->NodeLayerComboBox->setCurrentIndex(i);
            break;
        }
}
void NeatNode::setFunction(int function) {
    ui->NodeFunctionComboBox->setCurrentIndex(function);
}
void NeatNode::setFunctions(QStringList functions) {
    ui->NodeFunctionComboBox->addItems(functions);
}
void NeatNode::setType(int type) {
    ui->NodeTypeComboBox->setCurrentIndex(type);
}

// Getters
// -------

int NeatNode::getType() {
    return ui->NodeTypeComboBox->currentIndex();
}
int NeatNode::getFunction() {
    return ui->NodeFunctionComboBox->currentIndex();
}
int NeatNode::getLayer() {
    return ui->NodeLayerComboBox->currentText().toInt();
}
bool NeatNode::isChecked() {
    return ui->NodeCheckBox->isChecked();
}

// Slots
// -----

void NeatNode::sendLayerChange() {
    emit layerChange(ui->NodeLabel->text().toInt());
}
void NeatNode::sendFunctionChange() {
    emit functionChange(ui->NodeLabel->text().toInt());
}
void NeatNode::sendTypeChange() {
    emit typeChanged(ui->NodeLabel->text().toInt());
}
