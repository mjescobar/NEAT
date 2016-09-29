#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Load the graphic configuration from 'mainwindow.ui'
    ui->setupUi(this);
    ui->spinBox2100->setValue(2);
    ui->spinBox2101->setValue(1);

    // User Specifications Slots & Signals
    connect(ui->pushButton1102, SIGNAL(clicked()), this, SLOT(clickDefaultUserSpecifications()));
    connect(ui->pushButton1103, SIGNAL(clicked()), this, SLOT(clickResetUserSpecifications()));

    // Genetic Encoding Slots & Signals
    // Automatic configuration
    connect(ui->pushButton2100, SIGNAL(clicked()), this, SLOT(clickLoadFileGeneticEncoding()));
    connect(ui->pushButton2101, SIGNAL(clicked()), this, SLOT(clickSaveFileGeneticEncoding()));
    connect(ui->pushButton2102, SIGNAL(clicked()), this, SLOT(clickAutogenerateGeneticEncoding()));

    // Manual configuration
    connect(ui->pushButton2200, SIGNAL(clicked()), this, SLOT(clickAddNodeGeneticEncoding()));
    connect(ui->pushButton2201, SIGNAL(clicked()), this, SLOT(clickAddConnectionGeneticEncoding()));
    connect(ui->pushButton2202, SIGNAL(clicked()), this, SLOT(clickDeleteSelectedGeneticEncoding()));

    // Creating User Specifications
    userSpecifications = new UserSpecifications();
    userSpecifications->loadDefaultValues();

    updateUIUserSpecifications();

    // Creating Genetic Encoding
    geneticEncoding = new GeneticEncoding();
    geneticEncoding->autogenerateGeneticEncoding( ui->spinBox2100->value(), ui->spinBox2101->value() );

    updateUIGeneticEncoding();
}

// SLOTS User Specifications
void MainWindow::clickDefaultUserSpecifications()       { userSpecifications->loadDefaultValues(); updateUIUserSpecifications(); }
void MainWindow::clickResetUserSpecifications()         { userSpecifications->resetValues(); updateUIUserSpecifications(); }

// SLOTS Genetic Encoding
void MainWindow::clickAutogenerateGeneticEncoding()     { geneticEncoding->autogenerateGeneticEncoding(ui->spinBox2100->value(), ui->spinBox2101->value()); updateUIGeneticEncoding(); }
void MainWindow::clickSaveFileGeneticEncoding()         { geneticEncoding->saveFile(); }
void MainWindow::clickLoadFileGeneticEncoding()         { geneticEncoding->loadFile(); updateUIGeneticEncoding(); }
void MainWindow::clickDeleteSelectedGeneticEncoding()   { deleteSelectedGeneticEncoding(); }
void MainWindow::clickAddNodeGeneticEncoding()          { addNodeGeneticEncoding(); }
void MainWindow::clickAddConnectionGeneticEncoding()    { addConnectionGeneticEncoding(); }

void MainWindow::changeNodeExist( int id )              { geneticEncoding->setNodeExist     ( id, qobject_cast<QSpinBox*>(ui->gridLayout221->itemAtPosition( id+1, 2 )->widget())->value() ); }
void MainWindow::changeNodeType( int id )               { geneticEncoding->setNodeType      ( id, qobject_cast<QSpinBox*>(ui->gridLayout221->itemAtPosition( id+1, 3 )->widget())->value() ); }
void MainWindow::changeNodeRow( int id )                { geneticEncoding->setNodeRow       ( id, qobject_cast<QSpinBox*>(ui->gridLayout221->itemAtPosition( id+1, 4 )->widget())->value() ); }
void MainWindow::changeNodeFunction( int id )           { geneticEncoding->setNodeFunction  ( id, qobject_cast<QLineEdit*>(ui->gridLayout221->itemAtPosition( id+1, 5 )->widget())->text() ); }

void MainWindow::changeConnectionExist( int inn )       { geneticEncoding->setConnectionExist   ( inn, qobject_cast<QSpinBox*>(ui->gridLayout222->itemAtPosition( inn+1, 2 )->widget())->value() ); }
void MainWindow::changeConnectionIn( int inn )          { geneticEncoding->setConnectionIn      ( inn, qobject_cast<QSpinBox*>(ui->gridLayout222->itemAtPosition( inn+1, 3 )->widget())->value() ); }
void MainWindow::changeConnectionOut( int inn )         { geneticEncoding->setConnectionOut     ( inn, qobject_cast<QSpinBox*>(ui->gridLayout222->itemAtPosition( inn+1, 4 )->widget())->value() ); }
void MainWindow::changeConnectionWeight( int inn )      { geneticEncoding->setConnectionWeight  ( inn, qobject_cast<QDoubleSpinBox*>(ui->gridLayout222->itemAtPosition( inn+1, 5 )->widget())->value() ); }
void MainWindow::changeConnectionEnable( int inn )      { geneticEncoding->setConnectionEnable  ( inn, qobject_cast<QSpinBox*>(ui->gridLayout222->itemAtPosition( inn+1, 6 )->widget())->value() ); }

// METHODS User Specifications

void MainWindow::updateUIUserSpecifications()
{
    ui->doubleSpinBox11000->setValue( userSpecifications->getPopulationMax() );
    ui->doubleSpinBox11001->setValue( userSpecifications->getDistanceConst1() );
    ui->doubleSpinBox11002->setValue( userSpecifications->getDistanceConst2() );
    ui->doubleSpinBox11003->setValue( userSpecifications->getDistanceConst3() );
    ui->doubleSpinBox11004->setValue( userSpecifications->getDistanceThreshold() );
    ui->doubleSpinBox11005->setValue( userSpecifications->getPercentageOffspringWithoutCrossover() );
    ui->doubleSpinBox11006->setValue( userSpecifications->getProbabilityInterspaciesMating() );
    ui->doubleSpinBox11007->setValue( userSpecifications->getSmallerPopulationsProbabilityAddingNewNode() );
    ui->doubleSpinBox11008->setValue( userSpecifications->getSmallerPopulationsProbabilityAddingNewConnection() );
    ui->doubleSpinBox11009->setValue( userSpecifications->getLargerPopulationsProbabilityAddingNewNode() );
    ui->doubleSpinBox11010->setValue( userSpecifications->getLargerPopulationsProbabilityAddingNewConnection() );
    ui->doubleSpinBox11011->setValue( userSpecifications->getProbEnableAnDisableConnection() );
    ui->spinBox11012->setValue      ( userSpecifications->getLargePopulationDiscriminator() );
    ui->spinBox11013->setValue      ( userSpecifications->getGenerations() );
    ui->doubleSpinBox11014->setValue( userSpecifications->getProbabilityChangeWeight() );
    ui->doubleSpinBox11015->setValue( userSpecifications->getProbabilityChangeNodeFunctionPerNode() );
}

// METHODS Genetic Encoding

void MainWindow::updateUIGeneticEncoding()
{
    ui->spinBox2100->setValue( geneticEncoding->getInputNum() );
    ui->spinBox2101->setValue( geneticEncoding->getOutputNum() );

    // Emptying grid layout

    QLayoutItem * item;

    // Deleting nodes
    while ( ( item = ui->gridLayout221->takeAt(0) ) != 0 )
    {
        delete item->widget();
        delete item;
    }

    // Deleting connections
    while ( ( item = ui->gridLayout222->takeAt(0) ) != 0 )
    {
        delete item->widget();
        delete item;
    }

    // Filling grid layout

    // Creating nodes
    ui->gridLayout221->addWidget( new QLabel("Node"),        0, 1 );
    ui->gridLayout221->addWidget( new QLabel("Exist"),       0, 2 );
    ui->gridLayout221->addWidget( new QLabel("Type"),        0, 3 );
    ui->gridLayout221->addWidget( new QLabel("Row"),         0, 4 );
    ui->gridLayout221->addWidget( new QLabel("Function"),    0, 5 );

    for( int i = 0; i < geneticEncoding->getNodeSize(); i++ )
    {
        // Creating the widgets

        QCheckBox * select_check_box = new QCheckBox();

        QLabel * node_id = new QLabel( QString::number(i) );
        node_id->setAlignment(Qt::AlignHCenter);

        if ( !geneticEncoding->isNodeTaken(i) )
        {
            select_check_box->setDisabled( true );
            node_id->setText( "<font color=#AAAAAA>" + node_id->text() + "</font>" );
            ui->gridLayout221->addWidget( select_check_box,             i+1, 0 );
            ui->gridLayout221->addWidget( node_id,                      i+1, 1 );
            ui->gridLayout221->addWidget( new QLabel( "<font color=#AAAAAA>Not Assigned</font>" ), i+1, 2 );
            continue;
        }

        QSpinBox * exist_spin = new QSpinBox();
        exist_spin->setMaximum( 1 );
        exist_spin->setValue( geneticEncoding->getNodeExist(i) );

        QSpinBox * type_spin = new QSpinBox();
        type_spin->setMaximum( 2 );
        type_spin->setValue( geneticEncoding->getNodeType(i) );

        QSpinBox * row_spin = new QSpinBox();
        row_spin->setMaximum( 1000000 );
        row_spin->setValue( geneticEncoding->getNodeRow(i) );

        QLineEdit * functoin_edit = new QLineEdit( geneticEncoding->getNodeFunction(i) );

        // Positioning the widgets

        ui->gridLayout221->addWidget( select_check_box, i+1, 0 );
        ui->gridLayout221->addWidget( node_id,          i+1, 1 );
        ui->gridLayout221->addWidget( exist_spin,       i+1, 2 );
        ui->gridLayout221->addWidget( type_spin,        i+1, 3 );
        ui->gridLayout221->addWidget( row_spin,         i+1, 4 );
        ui->gridLayout221->addWidget( functoin_edit,    i+1, 5 );

        // Making the connections

        mapperNodeExist.append( new QSignalMapper(this) );
        mapperNodeType.append( new QSignalMapper(this) );
        mapperNodeRow.append( new QSignalMapper(this) );
        mapperNodeFunction.append( new QSignalMapper(this) );

        connect( exist_spin,    SIGNAL(valueChanged(int)), mapperNodeExist.last(), SLOT(map()) );
        connect( type_spin,     SIGNAL(valueChanged(int)), mapperNodeType.last(), SLOT(map()) );
        connect( row_spin,      SIGNAL(valueChanged(int)), mapperNodeRow.last(), SLOT(map()) );
        connect( functoin_edit, SIGNAL(textChanged(QString)),  mapperNodeFunction.last(), SLOT(map()) );

        mapperNodeExist.last()->setMapping     ( exist_spin, i );
        mapperNodeType.last()->setMapping      ( type_spin, i );
        mapperNodeRow.last()->setMapping       ( row_spin, i );
        mapperNodeFunction.last()->setMapping  ( functoin_edit, i );

        connect(mapperNodeExist.last(),      SIGNAL(mapped(int)),   this, SLOT(changeNodeExist(int)));
        connect(mapperNodeType.last(),       SIGNAL(mapped(int)),   this, SLOT(changeNodeType(int)));
        connect(mapperNodeRow.last(),        SIGNAL(mapped(int)),   this, SLOT(changeNodeRow(int)));
        connect(mapperNodeFunction.last(),   SIGNAL(mapped(int)),   this, SLOT(changeNodeFunction(int)));
    }

    // Creatin connections
    ui->gridLayout222->addWidget(new QLabel("Innovation"),  0, 1);
    ui->gridLayout222->addWidget(new QLabel("Exist"),       0, 2);
    ui->gridLayout222->addWidget(new QLabel("In"),          0, 3);
    ui->gridLayout222->addWidget(new QLabel("Out"),         0, 4);
    ui->gridLayout222->addWidget(new QLabel("Weight"),      0, 5);
    ui->gridLayout222->addWidget(new QLabel("Enable"),      0, 6);

    for( int i = 0; i < geneticEncoding->getConnectionSize(); i++ )
    {
        // Creating the widgets

        QCheckBox * select_check_box = new QCheckBox();

        QLabel * innovation_inn = new QLabel( QString::number(i) );
        innovation_inn->setAlignment( Qt::AlignHCenter );

        if ( !geneticEncoding->isConnectionTaken(i) )
        {
            select_check_box->setDisabled( true );
            innovation_inn->setText( "<font color=#AAAAAA>" + innovation_inn->text() + "</font>" );
            ui->gridLayout222->addWidget( select_check_box,             i+1, 0 );
            ui->gridLayout222->addWidget( innovation_inn,               i+1, 1 );
            ui->gridLayout222->addWidget( new QLabel( "<font color=#AAAAAA>Not Assigned</font>" ), i+1, 2 );
            continue;
        }

        QSpinBox * exist_spin = new QSpinBox();
        exist_spin->setMaximum( 1 );
        exist_spin->setValue( geneticEncoding->getConnectionExist(i) );

        QSpinBox * in_spin = new QSpinBox();
        in_spin->setMaximum( 1000000 );
        in_spin->setValue( geneticEncoding->getConnectionIn(i) );

        QSpinBox * out_spin = new QSpinBox();
        out_spin->setMaximum( 1000000 );
        out_spin->setValue(  geneticEncoding->getConnectionOut(i) );

        QDoubleSpinBox * weight_spin = new QDoubleSpinBox();
        weight_spin->setMaximum( 1000000. );
        weight_spin->setDecimals( 7 );
        weight_spin->setValue(  geneticEncoding->getConnectionWeight(i) );

        QSpinBox * enable_spin = new QSpinBox();
        enable_spin->setMaximum( 1 );
        enable_spin->setValue(  geneticEncoding->getConnectionEnable(i) );

        // Positioning the widgets

        ui->gridLayout222->addWidget( select_check_box, i+1, 0);
        ui->gridLayout222->addWidget( innovation_inn,   i+1, 1);
        ui->gridLayout222->addWidget( exist_spin,       i+1, 2);
        ui->gridLayout222->addWidget( in_spin,          i+1, 3);
        ui->gridLayout222->addWidget( out_spin,         i+1, 4);
        ui->gridLayout222->addWidget( weight_spin,      i+1, 5);
        ui->gridLayout222->addWidget( enable_spin,      i+1, 6);

        // Making the connections

        mapperConnectionExist.append    ( new QSignalMapper(this) );
        mapperConnectionIn.append       ( new QSignalMapper(this) );
        mapperConnectionOut.append      ( new QSignalMapper(this) );
        mapperConnectionWeight.append   ( new QSignalMapper(this) );
        mapperConnectionEnable.append   ( new QSignalMapper(this) );

        connect( exist_spin,  SIGNAL(valueChanged(int)),    mapperConnectionExist.last(),  SLOT(map()) );
        connect( in_spin,     SIGNAL(valueChanged(int)),    mapperConnectionIn.last(),     SLOT(map()) );
        connect( out_spin,    SIGNAL(valueChanged(int)),    mapperConnectionOut.last(),    SLOT(map()) );
        connect( weight_spin, SIGNAL(valueChanged(int)),    mapperConnectionWeight.last(), SLOT(map()) );
        connect( enable_spin, SIGNAL(valueChanged(int)),    mapperConnectionEnable.last(), SLOT(map()) );

        mapperConnectionExist.last()->setMapping   ( exist_spin, i );
        mapperConnectionIn.last()->setMapping      ( in_spin, i );
        mapperConnectionOut.last()->setMapping     ( out_spin, i );
        mapperConnectionWeight.last()->setMapping  ( weight_spin, i );
        mapperConnectionEnable.last()->setMapping  ( enable_spin, i );

        connect(mapperConnectionExist.last(),    SIGNAL(mapped(int)),    this, SLOT(changeConnectionExist(int)));
        connect(mapperConnectionIn.last(),       SIGNAL(mapped(int)),    this, SLOT(changeConnectionIn(int)));
        connect(mapperConnectionOut.last(),      SIGNAL(mapped(int)),    this, SLOT(changeConnectionOut(int)));
        connect(mapperConnectionWeight.last(),   SIGNAL(mapped(int)),    this, SLOT(changeConnectionWeight(int)));
        connect(mapperConnectionEnable.last(),   SIGNAL(mapped(int)),    this, SLOT(changeConnectionEnable(int)));
    }
}

void MainWindow::deleteSelectedGeneticEncoding()
{
    // Remove selected nodes
    for( int i = 0; i < geneticEncoding->getNodeSize(); i++ )
        if( qobject_cast<QCheckBox*>(ui->gridLayout221->itemAtPosition( i+1, 0 )->widget())->isChecked() )
            geneticEncoding->removeNode( qobject_cast<QLabel*>(ui->gridLayout221->itemAtPosition( i+1, 1 )->widget())->text().toInt() );

    // Remove selected connections
    for( int i = 0; i < geneticEncoding->getConnectionSize(); i++ )
        if( qobject_cast<QCheckBox*>(ui->gridLayout222->itemAtPosition( i+1, 0 )->widget())->isChecked() )
            geneticEncoding->removeConnection( qobject_cast<QLabel*>(ui->gridLayout222->itemAtPosition( i+1, 1 )->widget())->text().toInt() );

    updateUIGeneticEncoding();
}

void MainWindow::addNodeGeneticEncoding()
{
    bool ok;
    int id = QInputDialog::getInt( this, "New node", "Node id:", 0, 0, 1000000, 1, &ok );

    if ( !ok ) return;

    // If there is nothing there, create node
    if( geneticEncoding->getNodeSize() <= id )
        geneticEncoding->addNode( id );

    // If there was something there, create node
    else if( !qobject_cast<QCheckBox*>(ui->gridLayout221->itemAtPosition( id+1, 0 )->widget())->isEnabled() )
        geneticEncoding->addNode( id );

    // If there is something there, retreat
    else return;

    updateUIGeneticEncoding();
}

void MainWindow::addConnectionGeneticEncoding()
{
    bool ok;
    int inn = QInputDialog::getInt( this, "New connection", "Innovation number:", 0, 0, 1000000, 1, &ok );

    if ( !ok ) return;

    // If there is nothing there, create connection
    if( geneticEncoding->getConnectionSize() <= inn )
        geneticEncoding->addConnection( inn );

    // If there was something there, create connection
    else if( !qobject_cast<QCheckBox*>(ui->gridLayout222->itemAtPosition( inn+1, 0 )->widget())->isEnabled() )
        geneticEncoding->addConnection( inn );

    // If there is something there, retreat
    else return;

    updateUIGeneticEncoding();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete geneticEncoding;
}

