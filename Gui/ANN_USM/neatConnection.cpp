#include "neatConnection.h"
#include "ui_neatConnection.h"

NeatConnection::NeatConnection(int inn, QWidget *parent) : QWidget(parent), ui(new Ui::NeatConnection) {
    ui->setupUi(this);
    ui->ConnectionLabel->setText(QString::number(inn));

    connect(ui->ConnectionComboBoxIn, SIGNAL(currentIndexChanged(int)), this, SLOT(sendInChange()));
    connect(ui->ConnectionComboBoxOut, SIGNAL(currentIndexChanged(int)), this, SLOT(sendOutChange()));
    connect(ui->ConnectionDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sendWeightChange()));
    connect(ui->ConnectionCheckBoxEnable, SIGNAL(clicked(bool)), this, SLOT(sendEnableChange()));
}

NeatConnection::~NeatConnection() {
    delete ui;
}
void NeatConnection::NeatConnection::enable() {
    ui->ConnectionCheckBox->setDisabled(false);
    ui->ConnectionCheckBoxEnable->setDisabled(false);
    ui->ConnectionComboBoxIn->setDisabled(false);
    ui->ConnectionComboBoxOut->setDisabled(false);
}
void NeatConnection::disable() {
    ui->ConnectionCheckBox->setDisabled(true);
    ui->ConnectionCheckBoxEnable->setDisabled(true);
    ui->ConnectionComboBoxIn->setDisabled(true);
    ui->ConnectionComboBoxOut->setDisabled(true);
    ui->ConnectionDoubleSpinBox->setDisabled(true);
}
bool NeatConnection::isChecked() {
    return ui->ConnectionCheckBox->isChecked();
}

// Setters
// -------

void NeatConnection::setNodes(int nodes) {
    for( int i = 0; i < nodes; i++ )
    {
        ui->ConnectionComboBoxIn->addItem(QString::number(i));
        ui->ConnectionComboBoxOut->addItem(QString::number(i));
    }
}
void NeatConnection::setIn(int in) {
    ui->ConnectionComboBoxIn->setCurrentIndex(in);
}
void NeatConnection::setOut(int out) {
    ui->ConnectionComboBoxOut->setCurrentIndex(out);
}
void NeatConnection::setWeight(double weight) {
    ui->ConnectionDoubleSpinBox->setValue(weight);
}
void NeatConnection::setEnable(int enable) {
    ui->ConnectionCheckBoxEnable->setChecked((bool)enable);
}

// Getters
// -------

int NeatConnection::getIn() {
    return ui->ConnectionComboBoxIn->currentIndex();
}
int NeatConnection::getOut() {
    return ui->ConnectionComboBoxOut->currentIndex();
}
double NeatConnection::getWeight() {
    return ui->ConnectionDoubleSpinBox->value();
}
int NeatConnection::getEnable() {
    return (int)ui->ConnectionCheckBoxEnable->isChecked();
}

// Slot
// ----

void NeatConnection::sendInChange() {
    emit inChanged(ui->ConnectionLabel->text().toInt());
}
void NeatConnection::sendOutChange() {
    emit outChanged(ui->ConnectionLabel->text().toInt());
}
void NeatConnection::sendWeightChange() {
    emit weightChanged(ui->ConnectionLabel->text().toInt());
}
void NeatConnection::sendEnableChange() {
    emit enableChanged(ui->ConnectionLabel->text().toInt());
}
