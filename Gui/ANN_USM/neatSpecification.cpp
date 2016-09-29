#include "neatSpecification.h"
#include "ui_neatSpecification.h"

#include <QFileDialog>
#include <QTextStream>

NeatSpecification::NeatSpecification(QWidget *parent) : QWidget(parent), ui(new Ui::NeatSpecification) {
    ui->setupUi(this);

    // Default values
    setValues(false);

    // Connections
    connect(ui->SavePushButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(ui->LoadPushButton, SIGNAL(clicked()), this, SLOT(loadFile()));
}
NeatSpecification::~NeatSpecification() {
    delete ui;
}
void NeatSpecification::saveFile() {

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(0, "Save File", "/home", "User Definition (*.user_definition);;All files (*)", &selectedFilter, 0);

    // Extension by default .genetic_encoding
    if( fileName.split(".").last() != "user_definition" ) fileName.append(".user_definition");

    if (!fileName.isEmpty()) {

        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

        QTextStream out(&file);

        out << "POPULATION_MAX: " << ui->PMSpinBox->value() << endl;
        out << "DISTANCE_CONST_1: " << ui->DC1DoubleSpinBox->value() << endl;
        out << "DISTANCE_CONST_2: " << ui->DC2DoubleSpinBox->value() << endl;
        out << "DISTANCE_CONST_3: " << ui->DC3DoubleSpinBox->value() << endl;
        out << "DISTANCE_THRESHOLD: " << ui->DTSpinBox->value() << endl;
        out << "PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER: " << ui->POWCDoubleSpinBox->value() << endl;
        out << "PROBABILITY_INTERSPACIES_MATING: " << ui->PIMDoubleSpinBox->value() << endl;
        out << "SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE: " << ui->SPPANNDoubleSpinBox->value() << endl;
        out << "SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION: " << ui->SPPANCDoubleSpinBox->value() << endl;
        out << "LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE: " << ui->LPPANNDoubleSpinBox->value() << endl;
        out << "LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION: " << ui->LPPANCDoubleSpinBox->value() << endl;
        out << "PROB_ENABLE_AN_DISABLE_CONNECTION: " << ui->PEADCDoubleSpinBox->value() << endl;
        out << "LARGE_POPULATION_DISCRIMINATOR: " << ui->LPDSpinBox->value() << endl;
        out << "GENERATIONS: " << ui->GSpinBox->value() << endl;
        out << "PROBABILITY_CHANGE_WEIGHT: " << ui->PCWDoubleSpinBox->value() << endl;
        out << "PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE: " << ui->PCNFPNDoubleSpinBox->value() << endl;
        out << "ENABLE_CPPN_FUNCTIONS: " << ui->CPPNCheckBox->isChecked() << endl;
        out << "ENABLE_RECURRENT_CONNECTIONS: " << ui->RecurrentConnectionCheckBox->isChecked() << endl;
    }
}
void NeatSpecification::loadFile() {
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName( 0, "Load File", "/home", "User Definition (*.user_definition);;All files (*)", &selectedFilter, 0 );

    if ( !fileName.isEmpty() ) {
        QFile file(fileName);
        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) return;

        QTextStream in( &file );

        while ( !in.atEnd() ) {
            try {
                QString line = in.readLine();
                QStringList lstLine = line.split("\t");

                if( lstLine.first() == "POPULATION_MAX:")
                    ui->PMSpinBox->setValue( lstLine.last().toInt() );
                else if( lstLine.first() == "DISTANCE_CONST_1:")
                    ui->DC1DoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "DISTANCE_CONST_2:")
                    ui->DC2DoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "DISTANCE_CONST_3:")
                    ui->DC3DoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "DISTANCE_THRESHOLD:")
                    ui->DTSpinBox->setValue( lstLine.last().toInt() );
                else if( lstLine.first() == "PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER:")
                    ui->POWCDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "PROBABILITY_INTERSPACIES_MATING:")
                    ui->PIMDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE:")
                    ui->SPPANNDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION:")
                    ui->SPPANCDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE:")
                    ui->LPPANNDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION:")
                    ui->LPPANCDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "PROB_ENABLE_AN_DISABLE_CONNECTION:")
                    ui->PEADCDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "LARGE_POPULATION_DISCRIMINATOR:")
                    ui->LPDSpinBox->setValue( lstLine.last().toInt() );
                else if( lstLine.first() == "GENERATIONS:")
                    ui->GSpinBox->setValue( lstLine.last().toInt() );
                else if( lstLine.first() == "PROBABILITY_CHANGE_WEIGHT:")
                    ui->PCWDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE:")
                    ui->PCNFPNDoubleSpinBox->setValue( lstLine.last().toDouble() );
                else if( lstLine.first() == "ENABLE_CPPN_FUNCTIONS:")
                    ui->CPPNCheckBox->setChecked( (bool)lstLine.last().toInt() );
                else if( lstLine.first() == "ENABLE_RECURRENT_CONNECTIONS:")
                    ui->RecurrentConnectionCheckBox->setChecked( (bool)lstLine.last().toInt() );
            }
            catch(...){}
        }
    }
}

void NeatSpecification::setValues( bool reset ) {

    ui->PMSpinBox->setValue( ((reset == true) ? 0 :  150 ) );
    ui->DC1DoubleSpinBox->setValue( ((reset == true) ? 0 :  1.0 ) );
    ui->DC2DoubleSpinBox->setValue( ((reset == true) ? 0 :  1.0 ) );
    ui->DC3DoubleSpinBox->setValue( ((reset == true) ? 0 :  0.4 ) );
    ui->DTSpinBox->setValue( ((reset == true) ? 0 :  5 ) );
    ui->POWCDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.25 ) );
    ui->PIMDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.00001 ) );
    ui->SPPANNDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.0008 ) );
    ui->SPPANCDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.0008 ) );
    ui->LPPANNDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.0003 ) );
    ui->LPPANCDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.0006 ) );
    ui->PEADCDoubleSpinBox->setValue( ((reset == true) ? 0 :  0 ) );
    ui->LPDSpinBox->setValue( ((reset == true) ? 0 :  10 ) );
    ui->GSpinBox->setValue( ((reset == true) ? 0 :  100 ) );
    ui->PCWDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.3 ) );
    ui->PCNFPNDoubleSpinBox->setValue( ((reset == true) ? 0 :  0.1 ) );
    ui->CPPNCheckBox->setChecked( ((reset == true) ? false : true ) );
    ui->RecurrentConnectionCheckBox->setChecked( ((reset == true) ? false : true ) );
}



