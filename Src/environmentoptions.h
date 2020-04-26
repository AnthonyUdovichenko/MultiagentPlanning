#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H
#include "gl_const.h"

class EnvironmentOptions
{
public:
    EnvironmentOptions(/*bool AS, bool AD, bool CC, */int MT = CN_SP_MT_EUCL, int PT = CN_SP_PR_CUSTOM);
    //EnvironmentOptions();
    int     metrictype;     //Can be chosen Euclidean, Manhattan, Chebyshev and Diagonal distance
    //bool    allowsqueeze;   //Option that allows to move throught "bottleneck"
    //bool    allowdiagonal;  //Option that allows to make diagonal moves
    //bool    cutcorners;     //Option that allows to make diagonal moves, when one adjacent cell is untraversable
    int prioritytype;
};

#endif // ENVIRONMENTOPTIONS_H
