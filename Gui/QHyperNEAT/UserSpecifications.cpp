
#include "UserSpecifications.h"

using namespace user_specifications;

UserSpecifications::UserSpecifications()
{
    loadDefaultValues();
}

void UserSpecifications::loadDefaultValues()
{
    setValues( 150,1.0,1.0,0.4,3,25,0.00001,0.008,0.008,0.003,0.006,0,10,100,0.7,0.1 );
}

void UserSpecifications::resetValues()
{
    setValues( 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 );
}

void UserSpecifications::setValues( double pm, double dc1, double dc2, double dc3, double dt, double powc, double pim, double sppann, double sppanc, double lppann, double lppanc, double peadc, int lpd, int g, double pcw, double pcnfrn )
{
    POPULATION_MAX = pm;
    DISTANCE_CONST_1 = dc1;
    DISTANCE_CONST_2 = dc2;
    DISTANCE_CONST_3 = dc3;
    DISTANCE_THRESHOLD = dt;
    PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER = powc;
    PROBABILITY_INTERSPACIES_MATING = pim;
    SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE = sppann;
    SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION = sppanc;
    LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE = lppann;
    LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION = lppanc;
    PROB_ENABLE_AN_DISABLE_CONNECTION = peadc;
    LARGE_POPULATION_DISCRIMINATOR = lpd;
    GENERATIONS = g;
    PROBABILITY_CHANGE_WEIGHT = pcw;
    PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE = pcnfrn;
}

// Setter

void UserSpecifications::setPopulationMax( double pm )
{
    POPULATION_MAX = pm;
}

void UserSpecifications::setDistanceConst1( double dc1 )
{
    DISTANCE_CONST_1 = dc1;
}

void UserSpecifications::setDistanceConst2( double dc2 )
{
    DISTANCE_CONST_2 = dc2;
}

void UserSpecifications::setDistanceConst3( double dc3 )
{
    DISTANCE_CONST_3 = dc3;
}

void UserSpecifications::setDistanceThreshold( double dt )
{
    DISTANCE_THRESHOLD = dt;
}

void UserSpecifications::setPercentageOffspringWithoutCrossover( double powc )
{
    PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER = powc;
}

void UserSpecifications::setProbabilityInterspaciesMating( double pim )
{
    PROBABILITY_INTERSPACIES_MATING = pim;
}

void UserSpecifications::setSmallerPopulationsProbabilityAddingNewNode( double sppann )
{
    SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE = sppann;
}

void UserSpecifications::setSmallerPopulationsProbabilityAddingNewConnection( double sppanc )
{
    SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION = sppanc;
}

void UserSpecifications::setLargerPopulationsProbabilityAddingNewNode( double lppann )
{
    LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE = lppann;
}

void UserSpecifications::setLargerPopulationsProbabilityAddingNewConnection( double lppanc )
{
    LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION = lppanc;
}

void UserSpecifications::setProbEnableAnDisableConnection( double peadc )
{
    PROB_ENABLE_AN_DISABLE_CONNECTION = peadc;
}

void UserSpecifications::setLargePopulationDiscriminator( int lpd )
{
    LARGE_POPULATION_DISCRIMINATOR = lpd;
}

void UserSpecifications::setGenerations( int g )
{
    GENERATIONS = g;
}

void UserSpecifications::setProbabilityChangeWeight( double pcw )
{
    PROBABILITY_CHANGE_WEIGHT = pcw;
}

void UserSpecifications::setProbabilityChangeNodeFunctionPerNode( double pcfpn )
{
    PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE = pcfpn;
}

// Getters

double UserSpecifications::getPopulationMax()
{
    return POPULATION_MAX;
}

double UserSpecifications::getDistanceConst1()
{
    return DISTANCE_CONST_1;
}

double UserSpecifications::getDistanceConst2()
{
    return DISTANCE_CONST_2;
}

double UserSpecifications::getDistanceConst3()
{
    return DISTANCE_CONST_3;
}

double UserSpecifications::getDistanceThreshold()
{
    return DISTANCE_THRESHOLD;
}

double UserSpecifications::getPercentageOffspringWithoutCrossover()
{
    return PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER;
}

double UserSpecifications::getProbabilityInterspaciesMating()
{
    return PROBABILITY_INTERSPACIES_MATING;
}

double UserSpecifications::getSmallerPopulationsProbabilityAddingNewNode()
{
    return SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
}

double UserSpecifications::getSmallerPopulationsProbabilityAddingNewConnection()
{
    return SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
}

double UserSpecifications::getLargerPopulationsProbabilityAddingNewNode()
{
    return LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
}

double UserSpecifications::getLargerPopulationsProbabilityAddingNewConnection()
{
    return LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
}

double UserSpecifications::getProbEnableAnDisableConnection()
{
    return PROB_ENABLE_AN_DISABLE_CONNECTION;
}

int UserSpecifications::getLargePopulationDiscriminator()
{
    return LARGE_POPULATION_DISCRIMINATOR;
}

int UserSpecifications::getGenerations()
{
    return GENERATIONS;
}

double UserSpecifications::getProbabilityChangeWeight()
{
    return PROBABILITY_CHANGE_WEIGHT;
}

double UserSpecifications::getProbabilityChangeNodeFunctionPerNode()
{
    return PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE;
}

