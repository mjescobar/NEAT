#ifndef GENETICENCODING_H
#define GENETICENCODING_H

#include <QString>
#include <QVector>
#include <QFileDialog>
#include <QTextStream>
#include <iostream>

namespace genetic_encoding {

class GeneticEncoding
{
    public:

        GeneticEncoding();
        ~GeneticEncoding();

        // File management
        void saveFile();
        void loadFile();

        // Autogenerate from input/output
        void autogenerateGeneticEncoding( int input, int output );

        // -----
        // Nodes

        void addNode        ( int id, int exist, int type, int row, QString function );
        void addNode        ( int exist, int type, int row, QString function );
        void addNode        ( int id );
        void removeNode     ( int id );

        // Setters
        void setNodeExist   ( int id, int exist );
        void setNodeType    ( int id, int type );
        void setNodeRow     ( int id, int row );
        void setNodeFunction( int id, QString function );

        // Getters
        int getNodeExist   ( int id );
        int getNodeType    ( int id );
        int getNodeRow     ( int id );
        QString getNodeFunction( int id );

        // Misc
        int getNodeSize();
        bool isNodeTaken( int id );
        int getInputNum();
        int getOutputNum();

        // -----------
        // Connections

        void addConnection      ( int inn, int exist, int in, int out, double weight, int enable );
        void addConnection      ( int exist, int in, int out, double weight, int enable );
        void addConnection      ( int inn );
        void removeConnection   ( int inn );

        // Setters
        void setConnectionExist ( int inn, int exist );
        void setConnectionIn    ( int inn, int in );
        void setConnectionOut   ( int inn, int out );
        void setConnectionWeight( int inn, double weight );
        void setConnectionEnable( int inn, int enable );

        // Getters
        int getConnectionExist ( int inn );
        int getConnectionIn    ( int inn );
        int getConnectionOut   ( int inn );
        double getConnectionWeight( int inn );
        int getConnectionEnable( int inn );

        // Misc
        int getConnectionSize();
        bool isConnectionTaken( int inn );

        // Clear & free vectors' memory (for real)
        void clearVector( int mask );

    private:

        int input_num, output_num;

        // Nodes
        typedef struct {
            int id;
            int exist;
            int type;
            int row;
            QString function;
        } node;

        QVector< node * > nodes;

        // Connections
        typedef struct {
            int exist;
            int innovation;
            int in;
            int out;
            double weight;
            int enable;
        } connection;

        QVector< connection * > connections;
};

}

#endif // GENETICENCODING_H
