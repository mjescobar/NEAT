#include "neatLayer.h"
#include "ui_neatLayer.h"

NeatLayer::NeatLayer(QWidget *parent) : QWidget(parent), ui(new Ui::NeatLayer) {

    ui->setupUi(this);

    connect(ui->NEATLayerPushButton, SIGNAL(clicked()), this, SLOT(fixUpLineEdit()));
    //connect(ui->NEATLayerLineEdit, SIGNAL(returnPressed()), this, SLOT(fixUpLineEdit()));
    connect(ui->NEATLayerLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkLineEdit(QString)));
}

NeatLayer::~NeatLayer() {
    delete ui;
}

void NeatLayer::checkLineEdit(QString currentText) {
    int cursorPos = ui->NEATLayerLineEdit->cursorPosition();
    QString newText = currentText;

    // Delete all the garbage
    newText.replace(QRegExp("[^,0-9]"), "");
    ui->NEATLayerLineEdit->setText(newText);

    ui->NEATLayerLineEdit->setCursorPosition((newText.length() < currentText.length()) ? cursorPos-1 : cursorPos );
}

void NeatLayer::fixUpLineEdit() {
    QString layers = ui->NEATLayerLineEdit->text();

    // Remove numbers starting by "0+"
    layers.replace(QRegExp(",0+"), ",");

    // Remove the repeated commas
    layers.replace(QRegExp(",,+"), ",");

    // Remove border commas
    layers.replace(QRegExp("(^,|,$)"), "");

    // Remove repeated values
    QStringList layerList = layers.split(",");
    for( int i = 0; i < layerList.size(); i++ ) {
        // Remove the numbers 1 and 0
        // These numbers can only be at the beginning/end of the string
        if( layerList.at(i) == "0" || layerList.at(i) == "1" ) {
            layerList.removeAt(i--);
            continue;
        }

        // Delete if layer repeated
        for( int j = i + 1; j < layerList.size(); j++ )
            if( layerList.at(i) == layerList.at(j) )
                layerList.removeAt(j--);
    }
    layers = layerList.join(",");

    // Add the inital and final 0 & 1
    layers.push_front("0,");
    layers.push_back((layers.length() == 2) ? "1" : ",1");

    ui->NEATLayerLineEdit->setText(layers);

    // Emit the signal to NeatNode
    emit layerChanged();
}
// Setter
void NeatLayer::setLayers(QVector< int > layers) {
    QStringList layerList;
    for( int i = 0; i < layers.size(); i++)
        layerList.append(QString::number(layers.at(i)));
    ui->NEATLayerLineEdit->setText(layerList.join(","));
}
// Getter
QVector< int > NeatLayer::getLayers() {
    QStringList layerList = ui->NEATLayerLineEdit->text().split(",");
    QVector< int > layers;
    for( int i = 0; i < layerList.size(); i++)
        layers.append(layerList.at(i).toInt());

    return layers;
}
