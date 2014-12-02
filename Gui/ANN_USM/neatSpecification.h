#ifndef NEATSPECIFICATION_H
#define NEATSPECIFICATION_H

#include <QWidget>

namespace Ui {
class NeatSpecification;
}

class NeatSpecification : public QWidget
{
    Q_OBJECT

public:
    explicit NeatSpecification(QWidget *parent = 0);
    ~NeatSpecification();

    void setValues( bool reset );

public slots:

    void saveFile();
    void loadFile();

private:

    Ui::NeatSpecification *ui;

    int POPULATION_MAX;
    double DISTANCE_CONST_1;
    double DISTANCE_CONST_2;
    double DISTANCE_CONST_3;
    int DISTANCE_THRESHOLD;
    int PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER;
    double PROBABILITY_INTERSPACIES_MATING;
    double SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
    double SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
    double LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
    double LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
    double PROB_ENABLE_AN_DISABLE_CONNECTION;
    int LARGE_POPULATION_DISCRIMINATOR;
    int GENERATIONS;
    double PROBABILITY_CHANGE_WEIGHT;
    double PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE;
    bool ENABLE_CPPN_FUNCTIONS;
    bool ENABLE_RECURRENT_CONNECTIONS;
};

#endif // NEATSPECIFICATION_H
