#ifndef HYPERNEAT_H
#define HYPERNEAT_H

#include <QVector>

namespace hyper_neat
{



class Layout
{
    public:

        Layout();
        ~Layout();

        // Getters
        int getType( int x, int y );
        double getX( int x, int y );
        double getY( int x, int y );
        int getRefNum( int x, int y );
        int getXMax();
        int getXMin();
        int getYMax();
        int getYMin();
        int getXDiv();
        int getYDiv();

        // Setters
        void setType( int type, int x, int y );
        void setX( int X, int x, int y );
        void setY( int Y, int x, int y );
        void setRefNum( int ref_num, int x, int y );
        void setXMax( int xmax );
        void setXMin( int xmin );
        void setYMax( int ymax );
        void setYMin( int ymin );
        void setXDiv( int xdiv );
        void setYDiv( int ydiv );

    //private slots:
    private:

        int xMax, xMin, xDiv, yMax, yMin, yDiv;

        // Structure of a spot
        typedef struct
        {
            double x, y;    // Coordinate
            int type;       // 0:Input 1:Hidden 2:Output
            int ref_num;    // Mysterious number
        } koht;

        // Layout grid
        QVector< QVector< koht > >  layout;
};

class HyperNEAT
{
    public:



    //private slots:
    private:

        int layout_num;
        int layout_type;


};

}

#endif // HYPERNEAT_H
