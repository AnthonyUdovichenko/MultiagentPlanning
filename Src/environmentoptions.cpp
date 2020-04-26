#include "environmentoptions.h"

/*EnvironmentOptions::EnvironmentOptions()
{
    metrictype = CN_SP_MT_EUCL;
    prioritytype = CN_SP_PR_CUSTOM;
}*/

EnvironmentOptions::EnvironmentOptions(/*bool AS, bool AD, bool CC, */int MT, int PT)
{
    metrictype = MT;
    /*allowsqueeze = AS;
    allowdiagonal = AD;
    cutcorners = CC;*/
    prioritytype = PT;
}

