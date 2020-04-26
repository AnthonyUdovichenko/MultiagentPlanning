#include "mission.h"
#include <iostream>

Mission::Mission()
{
    logger = nullptr;
    fileName = nullptr;
}

Mission::Mission(const char *FileName)
{
    fileName = FileName;
    logger = nullptr;
}

Mission::~Mission()
{
    if (logger)
        delete logger;
}

bool Mission::getMap()
{
    return map.getMap(fileName);
}

bool Mission::getConfig()
{
    return config.getConfig(fileName);
}

bool Mission::createLog()
{
    if (logger != nullptr) delete logger;
    logger = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    return logger->getLog(fileName, config.LogParams);
}

void Mission::createEnvironmentOptions()
{
    //options.cutcorners = config.SearchParams[CN_SP_CC];
    //options.allowsqueeze = config.SearchParams[CN_SP_AS];
    //options.allowdiagonal = config.SearchParams[CN_SP_AD];
    options.metrictype = config.SearchParams[CN_SP_MT];
    options.prioritytype = config.SearchParams[CN_SP_PR];
}

void Mission::createSearch()
{
//might be helpful in case numerous algorithms are added
}

void Mission::startSearch()
{
    sr = search.startSearch(logger, map, options, config.SearchParams[CN_SP_ST], config.SearchParams[CN_SP_HW]);
}

void Mission::printSearchResultsToConsole()
{
    int agents = map.getAgents();
    for (int agent = 0; agent < agents; ++agent) {
        std::cout << "Agent " << agent + 1 << std::endl;
        std::cout << "Path ";
        if (!sr[agent].pathfound)
            std::cout << "NOT ";
        std::cout << "found!" << std::endl;
        std::cout << "numberofsteps=" << sr[agent].numberofsteps << std::endl;
        std::cout << "nodescreated=" << sr[agent].nodescreated << std::endl;
        if (sr[agent].pathfound) {
            std::cout << "pathlength=" << sr[agent].pathlength << std::endl;
            std::cout << "pathlength_scaled=" << sr[agent].pathlength * map.getCellSize() << std::endl;
        }
        std::cout << "time=" << sr[agent].time << std::endl;
    }
}

void Mission::saveSearchResultsToLog()
{
    int agents = map.getAgents();
    for (int agent = 0; agent < agents; ++agent) {
        logger->writeAgent(fileName, agent);

        logger->writeToLogSummary(sr[agent].numberofsteps, sr[agent].nodescreated, sr[agent].pathlength, sr[agent].time, map.getCellSize());
        if (sr[agent].pathfound) {
            logger->writeToLogPath(*sr[agent].lppath);
            logger->writeToLogHPpath(*sr[agent].hppath);
            logger->writeToLogMap(map, *sr[agent].lppath);
        } else
            logger->writeToLogNotFound();
    }
    logger->saveLog();
}

/*SearchResult Mission::getSearchResult()
{
    return sr;
}*/

