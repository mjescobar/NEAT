#include "mainWindow.h"
#include "neatStructure.h"
#include "neatSpecification.h"
#include "ui_mainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->NEATVStructureVerticalLayout->addWidget( new NeatStructure );
    ui->NEATSpecificationsVerticalLayout->addWidget( new NeatSpecification );
}

MainWindow::~MainWindow()
{
    delete ui;
}
