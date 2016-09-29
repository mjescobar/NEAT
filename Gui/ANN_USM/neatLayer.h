#ifndef NEATLAYER_H
#define NEATLAYER_H

#include <QWidget>
#include <QVector>

namespace Ui {
class NeatLayer;
}

class NeatLayer : public QWidget
{
    Q_OBJECT

public:

    explicit NeatLayer(QWidget *parent = 0);
    ~NeatLayer();

    // Setter
    void setLayers(QVector< int > layers);

    // Getter
    QVector< int > getLayers();

signals:

    void layerChanged();

private slots:

    void checkLineEdit(QString currentText);
    void fixUpLineEdit();

private:

    Ui::NeatLayer *ui;
};

#endif // NEATLAYER_H
