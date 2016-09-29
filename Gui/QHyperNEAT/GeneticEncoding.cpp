
#include <GeneticEncoding.h>
#include <iostream>

using namespace genetic_encoding;

GeneticEncoding::GeneticEncoding()
{
    input_num = 0;
    output_num = 0;
}

GeneticEncoding::~GeneticEncoding()
{
    clearVector(3);
}

void GeneticEncoding::saveFile()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(0, "Save File", "/home", "Genetic Encoding (*.genetic_encoding);;JSON (*.json);;All files (*)", &selectedFilter, 0);

    // Extension by default
    if( fileName.split(".").last() != "genetic_encoding" && fileName.split(".").last() != "json" )
        fileName.append(".genetic_encoding");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

        QTextStream out(&file);

        out << "{" << endl;
        out << "\tGenetic_Encoding\":" << endl;
        out << "\t{" << endl;
        out << "\t\t\"row_order\":" << endl;
        out << "\t\t[" << endl;
        out << "\t\t\t0,1" << endl;
        out << "\t\t]," << endl;
        out << "\t\t\"nodes\":" << endl;
        out << "\t\t[" << endl;

        int size = nodes.size();

        for( int i = 0; i < size; i++)
        {
            if( !isNodeTaken( i ) ) continue;

            out << "\t\t\t{\"exist\":" <<  nodes.at(i)->exist;
            out << ",\"node\":" << nodes.at(i)->id;
            out << ",\"type\":" << nodes.at(i)->type;
            out << ",\"row\":" << nodes.at(i)->row;
            out << ",\"function\":\"" << nodes.at(i)->function << "\"";
            out << ((i + 1 == size) ? "}" : "},") << endl;
        }

        out << "\t\t]," << endl;
        out << "\t\t\"connection_genes\":" << endl;
        out << "\t\t[" << endl;

        size = connections.size();

        for( int i = 0; i < size; i++)
        {
            if( !isConnectionTaken( i ) ) continue;

            out << "\t\t\t{\"exist\":" <<  connections.at(i)->exist;
            out << ",\"innovation\":" << connections.at(i)->innovation;
            out << ",\"in\":" << connections.at(i)->in;
            out << ",\"out\":" << connections.at(i)->out;
            out << ",\"weight\":" << connections.at(i)->weight;
            out << ",\"enable\":" << connections.at(i)->enable;
            out << ((i + 1 == size) ? "}" : "},") << endl;
        }

        out << "\t\t]" << endl;
        out << "\t}" << endl;
        out << "}" << endl;
    }
}

void GeneticEncoding::loadFile()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName( 0, "Load File", "/home", "Genetic Encoding (*.genetic_encoding);;JSON (*.json);;All files (*)", &selectedFilter, 0 );

    if ( !fileName.isEmpty() )
    {
        QFile file(fileName);
        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) return;

        QTextStream in( &file );

        // ==================
        // Little JSON parser

        QString line = in.readAll();
        line = line.simplified();

        QStringList list = line.split("[");
        list.removeFirst();

        // Get row orders
        // QStringList row_order_list = list[0].split("]")[0].split(",");

        // Get nodes
        QStringList node_list = list[1].split("]")[0].split("{");
        for( int i = 0; i < node_list.size(); i++ ) node_list[i] = node_list[i].split("}")[0];
        node_list.removeFirst(); // Can i put this first, before the for statement?

        // Get connections
        QStringList connection_list = list[2].split("]")[0].split("{");
        for( int i = 0; i < connection_list.size(); i++ ) connection_list[i] = connection_list[i].split("}")[0];
        connection_list.removeFirst();

        // ==================

        clearVector(3);

        input_num = 0;
        output_num = 0;

        // Create nodes
        for( int i = 0; i < node_list.size(); i++)
        {
            QStringList node_str = node_list[i].split(",");

            int exist_val = (node_str[0].split(":")[1]).simplified().toInt();
            int node_val = (node_str[1].split(":")[1]).simplified().toInt();
            int type_val = (node_str[2].split(":")[1]).simplified().toInt();
            int row_val = (node_str[3].split(":")[1]).simplified().toInt();
            QString function_val = (node_str[4].split(":")[1]).simplified();
            function_val = function_val.split("\"")[1];

            addNode( node_val, exist_val, type_val, row_val, function_val );
        }

        // Create connections
        for(int i = 0; i < connection_list.size(); i++)
        {
            QStringList connection_str = connection_list[i].split(",");

            int exist_val = (connection_str[0].split(":")[1]).simplified().toInt();
            int innovation_val = (connection_str[1].split(":")[1]).simplified().toInt();
            int in_val = (connection_str[2].split(":")[1]).simplified().toInt();
            int out_val = (connection_str[3].split(":")[1]).simplified().toInt();
            double weight_val = (connection_str[4].split(":")[1]).simplified().toDouble();
            int enable_val = (connection_str[5].split(":")[1]).simplified().toInt();

            addConnection( innovation_val, exist_val, in_val, out_val, weight_val, enable_val );
        }
    }
}

void GeneticEncoding::autogenerateGeneticEncoding( int input, int output )
{
    if (input < 1 || output < 1) return;

    int num_nodes = input + output;

    clearVector(3);

    // Create new nodes
    for( int i = 0; i < input; i++ )
        addNode( 1,0,0,"sigmoid" );

    for( int i = input; i < num_nodes; i++ )
        addNode( 1,2,1,"sigmoid" );

    // Create new connections
    for(int i = 0; i < input; i++)
        for(int j = input; j < num_nodes; j++)
            addConnection(1,i,j,1.0,1);

    input_num = input;
    output_num = output;
}

// ===================================================================
// Nodes

void GeneticEncoding::addNode( int id, int exist, int type, int row, QString function )
{
    if( isNodeTaken(id) ) return;

    // Filling with null pointers untill position 'id'
    while( getNodeSize() <= id ) nodes.append( NULL );

    node * tmp_node = new node();

    tmp_node->id = id;
    tmp_node->exist = exist;
    tmp_node->type = type;
    tmp_node->row = row;
    tmp_node->function = function;

    nodes.replace(id, tmp_node);

    if( type == 0 ) input_num++;
    if( type == 2 ) output_num++;
}

void GeneticEncoding::addNode( int exist, int type, int row, QString function )
{
    addNode( getNodeSize(), exist, type, row, function );
}

void GeneticEncoding::addNode( int id )
{
    addNode( id, 0, 0, 0, "" );
}

void GeneticEncoding::removeNode( int id )
{
    if( !isNodeTaken(id) ) return;
    if( nodes.at(id)->type == 0 ) input_num--;
    if( nodes.at(id)->type == 2 ) output_num--;
    delete nodes.at(id);
    nodes.replace(id, NULL);

    // Delete connections which have this node as input or output
    for( int i = 0; i < getConnectionSize(); i++)
        if( connections.at(i) != NULL )
            if( connections.at(i)->in == id || connections.at(i)->out == id )
                removeConnection( i );
}

// Node Setters

void GeneticEncoding::setNodeExist( int id, int exist )
{
    if( !isNodeTaken(id) ) return;
    nodes.at(id)->exist = exist;
}

void GeneticEncoding::setNodeType( int id, int type )
{
    if( !isNodeTaken(id) ) return;
    nodes.at(id)->type = type;
}

void GeneticEncoding::setNodeRow( int id, int row )
{
    if( !isNodeTaken(id) ) return;
    nodes.at(id)->row = row;
}

void GeneticEncoding::setNodeFunction( int id, QString function )
{
    if( !isNodeTaken(id) ) return;
    nodes.at(id)->function = function;
}

// Node Getters

int GeneticEncoding::getNodeExist( int id )
{
    if( !isNodeTaken(id) ) return -1;
    return nodes.at(id)->exist;
}

int GeneticEncoding::getNodeType( int id )
{
    if( !isNodeTaken(id) ) return -1;
    return nodes.at(id)->type;
}

int GeneticEncoding::getNodeRow( int id )
{
    if( !isNodeTaken(id) ) return -1;
    return nodes.at(id)->row;
}

QString GeneticEncoding::getNodeFunction( int id )
{
    if( !isNodeTaken(id) ) return "";
    return nodes.at(id)->function;
}

bool GeneticEncoding::isNodeTaken( int id )
{
    if( getNodeSize() <= id ) return false;
    if( nodes.at(id) == NULL ) return false;
    return true;
}

int GeneticEncoding::getInputNum()
{
    return input_num;
}

int GeneticEncoding::getOutputNum()
{
    return output_num;
}

int GeneticEncoding::getNodeSize()
{
    return nodes.size();
}

// ===================================================================
// Connection

void GeneticEncoding::addConnection( int inn, int exist, int in, int out, double weight, int enable )
{
    if( isConnectionTaken(inn) ) return;

    // Filling with null pointers untill position 'inn'
    while( connections.size() <= inn ) connections.append(NULL);

    connection * tmp_connection = new connection();

    tmp_connection->innovation = inn;
    tmp_connection->exist = exist;
    tmp_connection->in = in;
    tmp_connection->out = out;
    tmp_connection->weight = weight;
    tmp_connection->enable = enable;

    connections.replace(inn, tmp_connection);
}

void GeneticEncoding::addConnection( int exist, int in, int out, double weight, int enable )
{
    addConnection( connections.size(), exist, in, out, weight, enable );
}

void GeneticEncoding::addConnection( int inn )
{
    addConnection( inn, 0, 0, 0, 0., 0 );
}

void GeneticEncoding::removeConnection( int inn )
{
    if( !isConnectionTaken(inn) ) return;
    delete connections.at(inn);
    connections.replace(inn, NULL);
}

// Connection Setters

void GeneticEncoding::setConnectionExist( int inn, int exist )
{
    if( !isConnectionTaken(inn) ) return;
    connections.at(inn)->exist = exist;
}

void GeneticEncoding::setConnectionIn( int inn, int in )
{
    if( !isConnectionTaken(inn) ) return;
    connections.at(inn)->in = in;
}

void GeneticEncoding::setConnectionOut( int inn, int out )
{
    if( !isConnectionTaken(inn) ) return;
    connections.at(inn)->out = out;
}

void GeneticEncoding::setConnectionWeight( int inn, double weight )
{
    if( !isConnectionTaken(inn) ) return;
    connections.at(inn)->weight = weight;
}

void GeneticEncoding::setConnectionEnable( int inn, int enable )
{
    if( !isConnectionTaken(inn) ) return;
    connections.at(inn)->enable = enable;
}

// Connection Getters

int GeneticEncoding::getConnectionExist( int inn )
{
    if( !isConnectionTaken(inn) ) return -1;
    return connections.at(inn)->exist;
}

int GeneticEncoding::getConnectionIn( int inn )
{
    if( !isConnectionTaken(inn) ) return -1;
    return connections.at(inn)->in;
}

int GeneticEncoding::getConnectionOut( int inn )
{
    if( !isConnectionTaken(inn) ) return -1;
    return connections.at(inn)->out;
}

double GeneticEncoding::getConnectionWeight( int inn )
{
    if( !isConnectionTaken(inn) ) return 0;
    return connections.at(inn)->weight;
}

int GeneticEncoding::getConnectionEnable( int inn )
{
    if( !isConnectionTaken(inn) ) return -1;
    return connections.at(inn)->enable;
}

bool GeneticEncoding::isConnectionTaken( int inn )
{
    if( getConnectionSize() <= inn ) return false;
    if( connections.at(inn) == NULL ) return false;
    return true;
}

int GeneticEncoding::getConnectionSize()
{
    return connections.size();
}

// Clear vectors

void GeneticEncoding::clearVector( int32_t mask )
{
    // mask = 0b01 --> Clear only Nodes' vector
    // mask = 0b10 --> Clear only Connections' vector
    // mask = 0b11 --> Clear both

    // Clear nodes vector
    if( (mask & 1) == 1 )
    {
        for( int i = 0; i < nodes.size(); i++ ) delete nodes.at(i);
        QVector< node * >().swap(nodes);
        input_num = 0;
        output_num = 0;
    }

    // Clear connections vector
    if( (mask & 2) == 2 )
    {
        for( int i = 0; i < connections.size(); i++ ) delete connections.at(i);
        QVector< connection * >().swap(connections);
    }
}
