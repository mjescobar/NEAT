#ifndef USERSPECIFICATIONS_H
#define USERSPECIFICATIONS_H

namespace user_specifications
{

class UserSpecifications
{
    public:

        UserSpecifications();

        void loadDefaultValues();
        void resetValues();
        void setValues( double pm, double dc1, double dc2, double dc3, double dt, double powc, double pim, double sppann, double sppanc, double lppann, double lppanc, double peadc, int lpd, int g, double pcw, double pcnfrn );

        // Setters

        void setPopulationMax( double pm );
        void setDistanceConst1( double dc1 );
        void setDistanceConst2( double dc2 );
        void setDistanceConst3( double dc3 );
        void setDistanceThreshold( double dt );
        void setPercentageOffspringWithoutCrossover( double powc );
        void setProbabilityInterspaciesMating( double pim );
        void setSmallerPopulationsProbabilityAddingNewNode( double sppann );
        void setSmallerPopulationsProbabilityAddingNewConnection( double sppanc );
        void setLargerPopulationsProbabilityAddingNewNode( double lppann );
        void setLargerPopulationsProbabilityAddingNewConnection( double lppanc );
        void setProbEnableAnDisableConnection( double peadc );
        void setLargePopulationDiscriminator( int lpd );
        void setGenerations( int g );
        void setProbabilityChangeWeight( double pcw );
        void setProbabilityChangeNodeFunctionPerNode( double pcfpn );

        // Getters

        double getPopulationMax();
        double getDistanceConst1();
        double getDistanceConst2();
        double getDistanceConst3();
        double getDistanceThreshold();
        double getPercentageOffspringWithoutCrossover();
        double getProbabilityInterspaciesMating();
        double getSmallerPopulationsProbabilityAddingNewNode();
        double getSmallerPopulationsProbabilityAddingNewConnection();
        double getLargerPopulationsProbabilityAddingNewNode();
        double getLargerPopulationsProbabilityAddingNewConnection();
        double getProbEnableAnDisableConnection();
        int getLargePopulationDiscriminator();
        int getGenerations();
        double getProbabilityChangeWeight();
        double getProbabilityChangeNodeFunctionPerNode();

    private:

        double POPULATION_MAX;
        double DISTANCE_CONST_1;
        double DISTANCE_CONST_2;
        double DISTANCE_CONST_3;
        double DISTANCE_THRESHOLD;
        double PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER;
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
};

}

#endif // USERSPECIFICATIONS_H
