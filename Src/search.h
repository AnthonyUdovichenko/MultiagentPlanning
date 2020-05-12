#ifndef SEARCH_H
#define SEARCH_H
#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "tuplehash.h"
#include <list>
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
#include <unordered_set>

class Search
{
    public:
        Search();
        ~Search(void);
        std::vector<SearchResult> startSearch(ILogger *Logger, const Map &Map, const EnvironmentOptions &options, int algorithm, int heuristic_weight);

    protected:
        //CODE HERE
        static double heuristic(const Node &node, const Map &map, const EnvironmentOptions &options, int algorithm, int agent_number);
        static std::vector<Node> generateAllSuccs(Node &cur, const Map &map, const EnvironmentOptions &options, int algorithm, int heuristic_weight, std::unordered_map<std::tuple<int, int, int>, std::pair<int, int>, TupleHash> &moving_agents, int agent_num);
        SearchResult agentSearch(const Map &map, const EnvironmentOptions &options, int algorithm, int heuristic_weight, int agent_num, std::unordered_map<std::tuple<int, int, int>, std::pair<int, int>, TupleHash> &moving_agents);
        static int xDiff(Node &first, Node &second);
        static int yDiff(Node &first, Node &second);
        static void assignPriorities(const Map &map, const EnvironmentOptions &options, std::vector<int> &priorities);

        //Hint 1. You definitely need class variables for OPEN and CLOSE

        //Hint 2. It's a good idea to define a heuristic calculation function, that will simply return 0
        //for non-heuristic search methods like Dijkstra

        //Hint 3. It's a good idea to define function that given a node (and other stuff needed)
        //will return it's successors, e.g. unordered list of nodes

        //Hint 4. working with OPEN and CLOSE is the core
        //so think of the data structures that needed to be used, about the wrap-up classes (if needed)
        //Start with very simple (and ineffective) structures like list or vector and make it work first
        //and only then begin enhancement!


        std::vector<SearchResult>       sresult; //This will store the search result
        std::vector<std::list<Node>>    lppath, hppath; //

        //CODE HERE to define other members of the class
};
#endif
