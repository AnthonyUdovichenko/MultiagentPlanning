#include "search.h"

Search::Search()
{
//set defaults here
}

Search::~Search() {}

double Search::heuristic(const Node &node, const Map &map, const EnvironmentOptions &options, int algorithm, int agent_num)
{
    if (algorithm == CN_SP_ST_BFS || algorithm == CN_SP_ST_DIJK) {
        return 0;
    }

    if (options.metrictype == CN_SP_MT_MANH) {
        return abs(node.i - map.getGoalX(agent_num)) + abs(node.j - map.getGoalY(agent_num));
    }

    if (options.metrictype == CN_SP_MT_CHEB) {
        return std::max(abs(node.i - map.getGoalX(agent_num)), abs(node.j - map.getGoalY(agent_num)));
    }

    if (options.metrictype == CN_SP_MT_EUCL) {
        return sqrt(pow(node.i - map.getGoalX(agent_num), 2) + pow(node.j - map.getGoalY(agent_num), 2));
    }

    if (options.metrictype == CN_SP_MT_DIAG) {
        return CN_SQRT_TWO * std::min(abs(node.i - map.getGoalX(agent_num)), abs(node.j - map.getGoalY(agent_num))) +
               abs(abs(node.i - map.getGoalX(agent_num)) - abs(node.j - map.getGoalY(agent_num)));
    }

    return 0;
}

int argmin(const std::vector<Node> &OPEN) {
    if (OPEN.empty())
        return -1;
    int result = 0;
    double result_f = OPEN[0].F;
    for (int i = 1; i < (int)OPEN.size(); ++i) {
        if (OPEN[i].F < result_f) {
            result = i;
            result_f = OPEN[i].F;
        }
    }
    return result;
}

std::vector<Node> Search::generateAllSuccs(Node &cur, const Map &map, const EnvironmentOptions &options, int algorithm, int heuristic_weight, std::unordered_map<std::tuple<int, int, int>, std::pair<int, int>, TupleHash> &moving_agents, int agent_num) {
    std::vector<Node> SUCC = {};
    if (map.CellOnGrid(cur.i, cur.j - 1)) {
        if (map.CellIsTraversable(cur.i, cur.j - 1) && !moving_agents.count(std::make_tuple(cur.i, cur.j - 1, cur.time + 1))) {
            if (!(moving_agents.count(std::make_tuple(cur.i, cur.j, cur.time + 1)) && moving_agents[std::make_tuple(cur.i, cur.j, cur.time + 1)] == std::make_pair(cur.i, cur.j - 1))) {
                Node add{};
                add.i = cur.i;
                add.j = cur.j - 1;
                add.time = cur.time + 1;
                add.g = cur.g + 1;
                add.H = Search::heuristic(add, map, options, algorithm, agent_num);
                add.F = add.g + heuristic_weight * add.H;
                add.parent = {cur.i, cur.j};
                SUCC.push_back(add);
            }
        }
    }
    if (map.CellOnGrid(cur.i, cur.j + 1)) {
        if (map.CellIsTraversable(cur.i, cur.j + 1) && !moving_agents.count(std::make_tuple(cur.i, cur.j + 1, cur.time + 1))) {
            if (!(moving_agents.count(std::make_tuple(cur.i, cur.j, cur.time + 1)) && moving_agents[std::make_tuple(cur.i, cur.j, cur.time + 1)] == std::make_pair(cur.i, cur.j + 1))) {
                Node add{};
                add.i = cur.i;
                add.j = cur.j + 1;
                add.time = cur.time + 1;
                add.g = cur.g + 1;
                add.H = Search::heuristic(add, map, options, algorithm, agent_num);
                add.F = add.g + heuristic_weight * add.H;
                add.parent = {cur.i, cur.j};
                SUCC.push_back(add);
            }
        }
    }
    if (map.CellOnGrid(cur.i - 1, cur.j)) {
        if (map.CellIsTraversable(cur.i - 1, cur.j) && !moving_agents.count(std::make_tuple(cur.i - 1, cur.j, cur.time + 1))) {
            if (!(moving_agents.count(std::make_tuple(cur.i, cur.j, cur.time + 1)) && moving_agents[std::make_tuple(cur.i, cur.j, cur.time + 1)] == std::make_pair(cur.i - 1, cur.j))) {
                Node add{};
                add.i = cur.i - 1;
                add.j = cur.j;
                add.time = cur.time + 1;
                add.g = cur.g + 1;
                add.H = Search::heuristic(add, map, options, algorithm, agent_num);
                add.F = add.g + heuristic_weight * add.H;
                add.parent = {cur.i, cur.j};
                SUCC.push_back(add);
            }
        }
    }
    if (map.CellOnGrid(cur.i + 1, cur.j)) {
        if (map.CellIsTraversable(cur.i + 1, cur.j) && !moving_agents.count(std::make_tuple(cur.i + 1, cur.j, cur.time + 1))) {
            if (!(moving_agents.count(std::make_tuple(cur.i, cur.j, cur.time + 1)) && moving_agents[std::make_tuple(cur.i, cur.j, cur.time + 1)] == std::make_pair(cur.i + 1, cur.j))) {
                Node add{};
                add.i = cur.i + 1;
                add.j = cur.j;
                add.time = cur.time + 1;
                add.g = cur.g + 1;
                add.H = Search::heuristic(add, map, options, algorithm, agent_num);
                add.F = add.g + heuristic_weight * add.H;
                add.parent = {cur.i, cur.j};
                SUCC.push_back(add);
            }
        }
    }
    if (!moving_agents.count(std::make_tuple(cur.i, cur.j, cur.time + 1))) {
        Node add{};
        add.i = cur.i;
        add.j = cur.j;
        add.time = cur.time + 1;
        add.g = cur.g + 1;
        add.H = Search::heuristic(add, map, options, algorithm, agent_num);
        add.F = add.g + heuristic_weight * add.H;
        add.parent = {cur.i, cur.j};
        SUCC.push_back(add);
    }
    return SUCC;
}

int Search::xDiff(Node &first, Node &second) {
    return second.i - first.i;
}

int Search::yDiff(Node &first, Node &second) {
    return second.j - first.j;
}

struct PairHash {
    std::size_t operator() (const std::pair<int, int> &pair) const {
        return std::hash<std::size_t>() ((size_t) pair.first * 1000000000 + (size_t) pair.second);
    }
};

SearchResult Search::agentSearch(const Map &map, const EnvironmentOptions &options, int algorithm, int heuristic_weight, int agent_num, std::unordered_map<std::tuple<int, int, int>, std::pair<int, int>, TupleHash> &moving_agents)
{
    unsigned int start_time = clock();
    Node start{};
    start.i = map.getStartX(agent_num);
    start.j = map.getStartY(agent_num);
    start.time = 0;
    start.g = 0;
    start.H = heuristic(start, map, options, algorithm, agent_num);
    start.F = heuristic_weight * start.H;
    start.parent = {-1, -1};

    std::vector<Node> OPEN = {start};
    std::unordered_map<std::pair<int, int>, Node, PairHash> CLOSED;

    sresult[agent_num].numberofsteps = 0;
    while (!OPEN.empty()) {
        ++sresult[agent_num].numberofsteps;
        int cur_pos = argmin(OPEN);
        Node cur = OPEN[cur_pos];
        CLOSED[{cur.i, cur.j}] = cur;
        OPEN.erase(OPEN.begin() + cur_pos);
        if (cur.i == map.getGoalX(agent_num) && cur.j == map.getGoalY(agent_num)) {
            sresult[agent_num].pathfound = true;
            sresult[agent_num].pathlength = cur.g;
            lppath[agent_num].push_front(cur);
            moving_agents[std::make_tuple(cur.i, cur.j, cur.time)] = cur.parent;
            hppath[agent_num].push_front(cur);
            while (cur.parent != std::make_pair(-1, -1)) {
                cur = CLOSED.find({cur.parent.first, cur.parent.second})->second;
                if ((int)lppath[agent_num].size() >= 2 &&
                    (xDiff(*lppath[agent_num].begin(), *next(lppath[agent_num].begin())) != xDiff(cur, *lppath[agent_num].begin()) ||
                     yDiff(*lppath[agent_num].begin(), *next(lppath[agent_num].begin())) != yDiff(cur, *lppath[agent_num].begin()))) {
                    hppath[agent_num].push_front(*lppath[agent_num].begin());
                }
                lppath[agent_num].push_front(cur);
                moving_agents[std::make_tuple(cur.i, cur.j, cur.time)] = cur.parent;
                if (cur == start) {
                    hppath[agent_num].push_front(cur);
                }
            }
            sresult[agent_num].nodescreated = OPEN.size() + CLOSED.size();
            sresult[agent_num].lppath = &lppath[agent_num];
            sresult[agent_num].hppath = &hppath[agent_num];
            unsigned int end_time = clock();
            sresult[agent_num].time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
            return sresult[agent_num];
        }
        std::vector<Node> SUCC = generateAllSuccs(cur, map, options, algorithm, heuristic_weight, moving_agents, agent_num);
        for (Node &node : SUCC) {
            bool found_in_OPEN = false;
            for (Node &op : OPEN) {
                if (node.i == op.i && node.j == op.j) {
                    if (node.g < op.g) {
                        op.time = node.time;
                        op.g = node.g;
                        op.F = node.F;
                        op.parent = node.parent;
                    }
                    found_in_OPEN = true;
                    break;
                }
            }
            if (!found_in_OPEN) {
                bool found_in_CLOSED = (CLOSED.find({node.i, node.j}) != CLOSED.end());
                if (!found_in_CLOSED) {
                    OPEN.push_back(node);
                }
            }
        }
    }

    sresult[agent_num].pathfound = false;
    sresult[agent_num].pathlength = 0;
    /*sresult.nodescreated =  ;
    sresult.numberofsteps = ;
    sresult.time = ;*/
    sresult[agent_num].hppath = &hppath[agent_num];
    sresult[agent_num].lppath = &lppath[agent_num];
    unsigned int end_time = clock();
    sresult[agent_num].time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    return sresult[agent_num];
}

void Search::assignPriorities(const Map &map, const EnvironmentOptions &options, std::vector<int> &priorities)
{
    int agents = map.getAgents();
    if (options.prioritytype == CN_SP_PR_CUSTOM) {
        for (int i = 0; i < agents; ++i) {
            priorities[i] = i;
        }
    }
    ///to implement another priority types
}

std::vector<SearchResult> Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options, int algorithm, int heuristic_weight)
{
    int agents = map.getAgents();
    sresult.resize(agents);
    lppath.resize(agents);
    hppath.resize(agents);
    std::vector<int> priorities(agents);
    assignPriorities(map, options, priorities);
    std::unordered_map<std::tuple<int, int, int>, std::pair<int, int>, TupleHash> moving_agents; ///<now<x,y,time>,from<x,y>>
    for (int agent : priorities) {
        agentSearch(map, options, algorithm, heuristic_weight, agent, moving_agents);
    }
    return sresult;
}

/*void Search::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/
