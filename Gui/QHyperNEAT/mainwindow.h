#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <iostream>
#include <QFileDialog>
#include <QTextStream>
#include <QtWidgets>

#include "GeneticEncoding.h"
#include "UserSpecifications.h"

//using namespace std;
using namespace genetic_encoding;
using namespace user_specifications;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:

      // User Specifications

      void clickDefaultUserSpecifications();
      void clickResetUserSpecifications();

      // Genetic Encoding
      void clickAutogenerateGeneticEncoding();
      void clickSaveFileGeneticEncoding();
      void clickLoadFileGeneticEncoding();
      void clickDeleteSelectedGeneticEncoding();
      void clickAddNodeGeneticEncoding();
      void clickAddConnectionGeneticEncoding();

      void changeNodeExist( int id );
      void changeNodeType( int id );
      void changeNodeRow( int id );
      void changeNodeFunction( int id );

      void changeConnectionExist( int id );
      void changeConnectionIn( int id );
      void changeConnectionOut( int id );
      void changeConnectionWeight( int id );
      void changeConnectionEnable( int id );

    private:

        // Graphic elements
        Ui::MainWindow *ui;

        // User Specifications
        UserSpecifications * userSpecifications;

        void updateUIUserSpecifications();

        // Genetic Encoding
        GeneticEncoding * geneticEncoding;

        void updateUIGeneticEncoding();
        void deleteSelectedGeneticEncoding();
        void addNodeGeneticEncoding();
        void addConnectionGeneticEncoding();

        // Mappers
        QVector< QSignalMapper * > mapperNodeExist, mapperNodeType, mapperNodeRow, mapperNodeFunction;
        QVector< QSignalMapper * > mapperConnectionExist, mapperConnectionIn, mapperConnectionOut, mapperConnectionWeight, mapperConnectionEnable;
};

#endif // MAINWINDOW_H
