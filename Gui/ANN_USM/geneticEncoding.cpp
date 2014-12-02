#include "geneticEncoding.h"

#include <QFileDialog>
#include <QTextStream>

GeneticEncoding::GeneticEncoding(QObject *parent) : QObject(parent) {
    input_num = 0;
    output_num = 0;

    // WARNING! The order of the functions is extremly important
    strFunctions.append("IDENTITY");    // 0
    strFunctions.append("SIGMOID");     // 1
    strFunctions.append("GAUSSIAN");    // 2
    strFunctions.append("COS");         // 3
    strFunctions.append("SIN");         // 4

    // By default only Identity & Sigmoid
    cppn = false;
}
GeneticEncoding::~GeneticEncoding() {
    clearVector(3);
}
void GeneticEncoding::saveFile() {
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(0, "Save File", "/home", "Genetic Encoding (*.genetic_encoding);;All files (*)", &selectedFilter, 0);

    // Extension by default .genetic_encoding
    if( fileName.split(".").last() != "genetic_encoding" ) fileName.append(".genetic_encoding");

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

        QTextStream out(&file);

        // Save layers
        out << "l\t";
        for( int i = 0; i < layers.size(); i++)
            out << layers.at(i) << "\t";
        out << endl;

        // Save nodes
        int size = nodes.size();
        for( int i = 0; i < size; i++) {
            if( !existNode( i ) ) continue;

            out << "n\t" << nodes.at(i)->id;
            out << "\t" << nodes.at(i)->type;
            out << "\t" << nodes.at(i)->layer;
            out << "\t" << nodes.at(i)->function << endl;
        }

        // Save connections
        size = connections.size();
        for( int i = 0; i < size; i++) {
            if( !existConnection( i ) ) continue;

            out << "c\t" << connections.at(i)->innovation;
            out << "\t" << connections.at(i)->in;
            out << "\t" << connections.at(i)->out;
            out << "\t" << connections.at(i)->weight;
            out << "\t" << connections.at(i)->enable << endl;
        }
    }
}
void GeneticEncoding::loadFile() {
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName( 0, "Load File", "/home", "Genetic Encoding (*.genetic_encoding);;All files (*)", &selectedFilter, 0 );

    if ( !fileName.isEmpty() ) {
        QFile file(fileName);
        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) return;

        QTextStream in( &file );

        // Clear the three vectors (layers, nodes, connections)
        clearVector(7);

        while ( !in.atEnd() ) {
            try {
                QString line = in.readLine();
                QStringList lstLine = line.split("\t");

                // Load layers
                if( lstLine.first() == "l" )
                    for( int i = 1; i < lstLine.size()-1; i++ )
                        layers.append(lstLine.at(i).toInt());

                // Load nodes
                // ( Node, Type, Layer, Function )
                else if( lstLine.first() == "n" )
                    addNode( lstLine[1].toInt(), lstLine[2].toInt(), lstLine[3].toInt(), lstLine[4].toInt() );

                // Load connections
                // ( Innovation, In, Out, Weight, Enable )
                else if( lstLine.first() == "c" )
                    addConnection( lstLine[1].toInt(), lstLine[2].toInt(), lstLine[3].toInt(), lstLine[4].toDouble(), lstLine[5].toInt() );
            }
            catch(...){}
        }
    }
}

void GeneticEncoding::autogenerate( int input, int output ) {
    if (input < 1 || output < 1) return;

    int num_nodes = input + output;

    clearVector(7);

    // Create layers
    appendLayer(0);
    appendLayer(1);

    // Create new nodes
    for( int i = 0; i < input; i++ )
        addNode( 0, 0, 0 );
    for( int i = input; i < num_nodes; i++ )
        addNode( 2, 1, 1 );

    // Create new connections
    for(int i = 0; i < input; i++)
        for(int j = input; j < num_nodes; j++)
            addConnection( i, j, 1.0, 1 );

    input_num = input;
    output_num = output;
}

void GeneticEncoding::setCPPN( bool check ) {

    cppn = check;

    // Loop through all the nodes to reset their function
    if( cppn == false )
        for( int i = 0; i < nodes.size(); i++ )
            if( nodes.at(i)->function > 1)
                nodes.at(i)->function = 1;
}

// Layers
// ------

// Layers Setters
void GeneticEncoding::setLayers( QVector< int > layers) {
    this->layers = layers;
}
void GeneticEncoding::appendLayer( int layer ) {
    layers.append(layer);
}

// Layers Getter
QVector< int > GeneticEncoding::getLayers() {
    return layers;
}

// Nodes
// -----

void GeneticEncoding::addNode( int id, int type, int layer, int function ) {
    if( existNode(id) ) return;

    node * auxNode;

    // Fill with unexisted nodes
    while( getNodeSize() <= id ) {
        auxNode = new node;

        auxNode->exist = false;
        auxNode->id = getNodeSize();
        auxNode->type = 0;
        auxNode->layer = 0;
        auxNode->function = 0;

        nodes.append( auxNode );
    }

    auxNode = nodes.at(id);

    auxNode->exist = true;
    auxNode->type = type;
    auxNode->layer = layer;
    auxNode->function = function;

    if( type == 0 ) input_num++;
    if( type == 2 ) output_num++;
}
void GeneticEncoding::addNode(int type, int layer, int function ) {
    addNode( getNodeSize(), type, layer, function );
}
void GeneticEncoding::addNode( int id ) {
    addNode( id, 0, 0, 0 );
}
void GeneticEncoding::killNode ( int id ) {
    if( !existNode(id) ) return;

    if( nodes.at(id)->type == 0 ) input_num--;
    if( nodes.at(id)->type == 2 ) output_num--;

    nodes.at(id)->exist = false;

    // Delete the connections in which 'id' is present
    for( int i = 0; i < getConnectionSize(); i++)
        if( connections.at(i)->exist != false  )
            if( connections.at(i)->in == id || connections.at(i)->out == id )
                killConnection( i );
}
void GeneticEncoding::reviveNode ( int id ) {
    if( existNode(id) ) return;

    if( nodes.at(id)->type == 0 ) input_num++;
    if( nodes.at(id)->type == 2 ) output_num++;

    nodes.at(id)->exist = true;
}
bool GeneticEncoding::existNode( int id ) {
    if( getNodeSize() <= id ) return false;
    return nodes.at(id)->exist;
}

// Nodes Setters

void GeneticEncoding::setNodeType( int id, int type ) {
    if( !existNode(id) ) return;
    nodes.at(id)->type = type;
}
void GeneticEncoding::setNodeLayer( int id, int layer ) {
    if( !existNode(id) ) return;
    nodes.at(id)->layer = layer;
}
void GeneticEncoding::setNodeFunction( int id, int function ) {
    if( !existNode(id) ) return;
    nodes.at(id)->function = function;
}

// Nodes Getters

int GeneticEncoding::getNodeType( int id ) {
    if( !existNode(id) ) return -1;
    return nodes.at(id)->type;
}
int GeneticEncoding::getNodeLayer( int id ) {
    if( !existNode(id) ) return -1;
    return nodes.at(id)->layer;
}
int GeneticEncoding::getNodeFunction( int id ) {
    if( !existNode(id) ) return -1;
    return nodes.at(id)->function;
}
QString GeneticEncoding::getNodeStrFunction( int id ) {
    if( !existNode(id) ) return "--";
    return strFunctions.at( getNodeFunction(id) );
}

// Nodes Misc

int GeneticEncoding::getInputNum() {
    return input_num;
}
int GeneticEncoding::getOutputNum() {
    return output_num;
}
int GeneticEncoding::getNodeSize() {
    return nodes.size();
}

// ===================================================================
// Connection

void GeneticEncoding::addConnection( int inn, int in, int out, double weight, int enable ) {
    if( existConnection(inn) ) return;

    connection * auxConnection;

    while( getConnectionSize() <= inn ) {
        auxConnection = new connection;

        auxConnection->exist = false;
        auxConnection->innovation = getConnectionSize();
        auxConnection->in = 0;
        auxConnection->out = 1;
        auxConnection->weight = 0.;
        auxConnection->enable = false;

        connections.append( auxConnection );
    }

    auxConnection = connections.at(inn);

    auxConnection->exist = true;
    auxConnection->in = in;
    auxConnection->out = out;
    auxConnection->weight = weight;
    auxConnection->enable = enable;
}

void GeneticEncoding::addConnection( int in, int out, double weight, int enable ) {
    addConnection( getConnectionSize(), in, out, weight, enable );
}
void GeneticEncoding::addConnection( int inn ) {
    addConnection( inn, 0, 1, 1., 1 );
}
void GeneticEncoding::killConnection( int inn ) {
    if( !existConnection(inn) ) return;
    connections.at(inn)->exist = false;
}
void GeneticEncoding::reviveConnection( int inn ) {
    if( existConnection(inn) ) return;
    connections.at(inn)->exist = true;
}
bool GeneticEncoding::existConnection( int inn ) {
    if( getConnectionSize() <= inn ) return false;
    return connections.at(inn)->exist;
}

// Connection Setters

void GeneticEncoding::setConnectionIn( int inn, int in ) {
    if( !existConnection(inn) ) return;
    connections.at(inn)->in = in;
}
void GeneticEncoding::setConnectionOut( int inn, int out ) {
    if( !existConnection(inn) ) return;
    connections.at(inn)->out = out;
}
void GeneticEncoding::setConnectionWeight( int inn, double weight ) {
    if( !existConnection(inn) ) return;
    connections.at(inn)->weight = weight;
}
void GeneticEncoding::setConnectionEnable( int inn, int enable ) {
    if( !existConnection(inn) ) return;
    connections.at(inn)->enable = enable;
}

// Connection Getters

int GeneticEncoding::getConnectionIn( int inn ) {
    if( !existConnection(inn) ) return -1;
    return connections.at(inn)->in;
}
int GeneticEncoding::getConnectionOut( int inn ) {
    if( !existConnection(inn) ) return -1;
    return connections.at(inn)->out;
}
double GeneticEncoding::getConnectionWeight( int inn ) {
    if( !existConnection(inn) ) return 0;
    return connections.at(inn)->weight;
}
int GeneticEncoding::getConnectionEnable( int inn ) {
    if( !existConnection(inn) ) return -1;
    return connections.at(inn)->enable;
}

// Connection Misc

int GeneticEncoding::getConnectionSize() {
    return connections.size();
}

// Misc
// ----

QStringList GeneticEncoding::getStrFunctions() {
    if( cppn == true) return strFunctions;
    else {
        QStringList aux;
        aux.append(strFunctions.at(0));
        aux.append(strFunctions.at(1));
        return aux;
    }
}

// Clear vectors
void GeneticEncoding::clearVector( int32_t mask ) {
    // mask = 0b001 --> Clear layers
    // mask = 0b010 --> Clear nodes
    // mask = 0b100 --> Clear connections
    // Or any combination of those three

    // Clear layer vector
    if( (mask & 1) )
        QVector< int >().swap(layers);

    // Clear nodes vector
    if( (mask & 2) ) {
        for( int i = 0; i < nodes.size(); i++ ) delete nodes.at(i);
        QVector< node * >().swap(nodes);
        input_num = 0;
        output_num = 0;
    }

    // Clear connections vector
    if( (mask & 4) ) {
        for( int i = 0; i < connections.size(); i++ ) delete connections.at(i);
        QVector< connection * >().swap(connections);
    }
}
