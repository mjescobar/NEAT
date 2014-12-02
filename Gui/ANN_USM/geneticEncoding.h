#ifndef GENETICENCODING_H
#define GENETICENCODING_H

#include <QObject>
#include <QVector>
#include <QStringList>

/**
 * @brief The GeneticEncoding class - This class contains the main data structure used by the NeatStructure UI.
 */
class GeneticEncoding : public QObject
{
    Q_OBJECT

public:

    explicit GeneticEncoding(QObject *parent = 0);
    ~GeneticEncoding();

    // Layers
    // ------

    // Setters
    void setLayers( QVector< int > layers);
    void appendLayer( int layer );

    // Getter
    QVector< int > getLayers();

    // Nodes
    // -----

    void addNode                ( int id, int type, int layer, int function );
    void addNode                ( int type, int layer, int function );
    void addNode                ( int id );

    void killNode               ( int id );
    void reviveNode             ( int id );
    bool existNode              ( int id );

    // Setters
    void setNodeType            ( int id, int type );
    void setNodeLayer           ( int id, int layer );
    void setNodeFunction        ( int id, int function );

    // Getters
    int getNodeType             ( int id );
    int getNodeLayer            ( int id );
    int getNodeFunction         ( int id );
    QString getNodeStrFunction  ( int id );

    // Misc
    int getNodeSize();
    int getInputNum();
    int getOutputNum();

    // Connections
    // -----------

    void addConnection          ( int inn, int in, int out, double weight, int enable );
    void addConnection          ( int in, int out, double weight, int enable );
    void addConnection          ( int inn );

    void killConnection         ( int id );
    void reviveConnection       ( int id );
    bool existConnection        ( int id );

    // Setters
    void setConnectionIn        ( int inn, int in );
    void setConnectionOut       ( int inn, int out );
    void setConnectionWeight    ( int inn, double weight );
    void setConnectionEnable    ( int inn, int enable );

    // Getters
    int getConnectionIn         ( int inn );
    int getConnectionOut        ( int inn );
    double getConnectionWeight  ( int inn );
    int getConnectionEnable     ( int inn );

    // Misc
    int getConnectionSize();

    // Misc
    // ----

    // Return the functions list to be displayed
    QStringList getStrFunctions();

    // Clear & free vectors' memory (for real)
    void clearVector( int mask );

public slots:

    // File management
    void loadFile();
    void saveFile();

    // Autogenerate from input/output
    void autogenerate( int input, int output );

    // CPPN modality
    void setCPPN( bool check );

private:

    int input_num, output_num;

    // Nodes
    typedef struct {
        int id;
        int type;
        int layer;
        int function;
        bool exist;
    } node;

    // Connections
    typedef struct {
        int innovation;
        int in;
        int out;
        int enable;
        double weight;
        bool exist;
    } connection;

    QVector< int > layers;
    QVector< node * > nodes;
    QVector< connection * > connections;

    QStringList strFunctions;

    // This flag enables or disables the functions according to its value
    bool cppn;
};

#endif // GENETICENCODING_H
