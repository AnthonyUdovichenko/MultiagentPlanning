#include "map.h"


Map::Map()
{
    height = -1;
    width = -1;
    agents = 0;
    start_i = nullptr;
    start_j = nullptr;
    goal_i = nullptr;
    goal_j = nullptr;
    Grid = nullptr;
    cellSize = 1;
}

Map::~Map()
{
    if (Grid) {
        for (int i = 0; i < height; ++i)
            delete[] Grid[i];
        delete[] Grid;
    }
    delete[] start_i;
    delete[] start_j;
    delete[] goal_i;
    delete[] goal_j;
}

bool Map::CellIsTraversable(int i, int j) const
{
    return (Grid[i][j] == CN_GC_NOOBS);
}

bool Map::CellIsObstacle(int i, int j) const
{
    return (Grid[i][j] != CN_GC_NOOBS);
}

bool Map::CellOnGrid(int i, int j) const
{
    return (i < height && i >= 0 && j < width && j >= 0);
}

bool Map::getMap(const char *FileName)
{
    int rowiter = 0, grid_i = 0, grid_j = 0;

    tinyxml2::XMLElement *root = 0, *map = 0, *element = 0, *mapnode;

    std::string value;
    std::stringstream stream;

    bool hasGridMem = false, hasGrid = false, hasHeight = false, hasWidth = false, hasSTXMem = false, hasSTX = false, hasSTYMem = false, hasSTY = false, hasFINXMem = false, hasFINX = false, hasFINYMem = false, hasFINY = false, hasCellSize = false, hasAgents = false;

    tinyxml2::XMLDocument doc;

    // Load XML File
    if (doc.LoadFile(FileName) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cout << "Error opening XML file!" << std::endl;
        return false;
    }
    // Get ROOT element
    root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root) {
        std::cout << "Error! No '" << CNS_TAG_ROOT << "' tag found in XML file!" << std::endl;
        return false;
    }

    // Get MAP element
    map = root->FirstChildElement(CNS_TAG_MAP);
    if (!map) {
        std::cout << "Error! No '" << CNS_TAG_MAP << "' tag found in XML file!" << std::endl;
        return false;
    }

    for (mapnode = map->FirstChildElement(); mapnode; mapnode = mapnode->NextSiblingElement()) {
        element = mapnode->ToElement();
        value = mapnode->Value();
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        stream.str("");
        stream.clear();

        if (value != CNS_TAG_GRID)
        {
           stream << element->GetText();
        }


        if (!hasGridMem && hasHeight && hasWidth) {
            Grid = new int *[height];
            for (int i = 0; i < height; ++i)
                Grid[i] = new int[width];
            hasGridMem = true;
        }

        if (!hasSTXMem && hasAgents) {
            start_i = new int[agents];
            hasSTXMem = true;
        }

        if (!hasSTYMem && hasAgents) {
            start_j = new int[agents];
            hasSTYMem = true;
        }

        if (!hasFINXMem && hasAgents) {
            goal_i = new int[agents];
            hasFINXMem = true;
        }

        if (!hasFINYMem && hasAgents) {
            goal_j = new int[agents];
            hasFINYMem = true;
        }

        if (value == CNS_TAG_HEIGHT) {
            if (hasHeight) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_HEIGHT << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_HEIGHT << "' =" << height << "will be used."
                          << std::endl;
            }
            else {
                if (!((stream >> height) && (height > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_HEIGHT
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_HEIGHT << "' tag should be an integer >=0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_HEIGHT
                              << "' tag will be encountered later..." << std::endl;
                }
                else
                    hasHeight = true;
            }
        }
        else if (value == CNS_TAG_WIDTH) {
            if (hasWidth) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_WIDTH << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_WIDTH << "' =" << width << "will be used." << std::endl;
            }
            else {
                if (!((stream >> width) && (width > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_WIDTH
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_WIDTH << "' tag should be an integer AND >0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_WIDTH
                              << "' tag will be encountered later..." << std::endl;

                }
                else
                    hasWidth = true;
            }
        }
        else if (value == CNS_TAG_CELLSIZE) {
            if (hasCellSize) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_CELLSIZE << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_CELLSIZE << "' =" << cellSize << "will be used."
                          << std::endl;
            }
            else {
                if (!((stream >> cellSize) && (cellSize > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_CELLSIZE
                              << "' tag encountered (or could not convert to double)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_CELLSIZE
                              << "' tag should be double AND >0. By default it is defined to '1'" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_CELLSIZE
                              << "' tag will be encountered later..." << std::endl;
                }
                else
                    hasCellSize = true;
            }
        }
        else if (value == CNS_TAG_STX) {
            if (!hasWidth) {
                std::cout << "Error! '" << CNS_TAG_STX << "' tag encountered before '" << CNS_TAG_WIDTH << "' tag."
                          << std::endl;
                return false;
            }

            if (hasSTX) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_STX << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_STX << "' will be used." << std::endl;
            }
            else {
                std::vector<std::string> elems;
                std::string item;
                while (std::getline(stream, item, ' '))
                    elems.push_back(item);
                int agent_num = 0;
                int val;
                if (elems.size() > 0)
                    for (agent_num = 0; agent_num < agents; ++agent_num) {
                        if (agent_num == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[agent_num];
                        if (!(stream >> val && val >= 0 && val < width)) {
                            std::cout << "Warning! Invalid value of '" << CNS_TAG_STX
                                      << "' tag encountered (or could not convert to integer)" << std::endl;
                            std::cout << "Value of '" << CNS_TAG_STX << "' tag should be an integer AND >=0 AND < '"
                                      << CNS_TAG_WIDTH << "' value, which is " << width << std::endl;
                            return false;
                        }
                        start_i[agent_num] = val;
                    }

                if (agent_num != agents) {
                    std::cout << "Invalid number of '" << CNS_TAG_STX << "' arguments" << std::endl;
                    return false;
                }

                hasSTX = true;
            }
        }
        else if (value == CNS_TAG_STY) {
            if (!hasHeight) {
                std::cout << "Error! '" << CNS_TAG_STY << "' tag encountered before '" << CNS_TAG_HEIGHT << "' tag."
                          << std::endl;
                return false;
            }

            if (hasSTY) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_STY << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_STY << "' will be used." << std::endl;
            }
            else {
                std::vector<std::string> elems;
                std::string item;
                while (std::getline(stream, item, ' '))
                    elems.push_back(item);
                int agent_num = 0;
                int val;
                if (elems.size() > 0)
                    for (agent_num = 0; agent_num < agents; ++agent_num) {
                        if (agent_num == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[agent_num];
                        if (!(stream >> val && val >= 0 && val < height)) {
                            std::cout << "Warning! Invalid value of '" << CNS_TAG_STY
                                      << "' tag encountered (or could not convert to integer)" << std::endl;
                            std::cout << "Value of '" << CNS_TAG_STY << "' tag should be an integer AND >=0 AND < '"
                                      << CNS_TAG_HEIGHT << "' value, which is " << height << std::endl;
                            return false;
                        }
                        start_j[agent_num] = val;
                    }

                if (agent_num != agents) {
                    std::cout << "Invalid number of '" << CNS_TAG_STY << "' arguments" << std::endl;
                    return false;
                }

                hasSTY = true;
            }
        }
        else if (value == CNS_TAG_FINX) {
            if (!hasWidth) {
                std::cout << "Error! '" << CNS_TAG_FINX << "' tag encountered before '" << CNS_TAG_WIDTH << "' tag."
                          << std::endl;
                return false;
            }

            if (hasFINX) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_FINX << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_FINX << "' will be used." << std::endl;
            }
            else {
                std::vector<std::string> elems;
                std::string item;
                while (std::getline(stream, item, ' '))
                    elems.push_back(item);
                int agent_num = 0;
                int val;
                if (elems.size() > 0)
                    for (agent_num = 0; agent_num < agents; ++agent_num) {
                        if (agent_num == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[agent_num];
                        if (!(stream >> val && val >= 0 && val < width)) {
                            std::cout << "Warning! Invalid value of '" << CNS_TAG_FINX
                                      << "' tag encountered (or could not convert to integer)" << std::endl;
                            std::cout << "Value of '" << CNS_TAG_FINX << "' tag should be an integer AND >=0 AND < '"
                                      << CNS_TAG_WIDTH << "' value, which is " << width << std::endl;
                            return false;
                        }
                        goal_i[agent_num] = val;
                    }

                if (agent_num != agents) {
                    std::cout << "Invalid number of '" << CNS_TAG_FINX << "' arguments" << std::endl;
                    return false;
                }

                hasFINX = true;
            }
        }
        else if (value == CNS_TAG_FINY) {
            if (!hasHeight) {
                std::cout << "Error! '" << CNS_TAG_FINY << "' tag encountered before '" << CNS_TAG_HEIGHT << "' tag."
                          << std::endl;
                return false;
            }

            if (hasFINY) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_FINY << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_FINY << "' will be used." << std::endl;
            }
            else {
                std::vector<std::string> elems;
                std::string item;
                while (std::getline(stream, item, ' '))
                    elems.push_back(item);
                int agent_num = 0;
                int val;
                if (elems.size() > 0)
                    for (agent_num = 0; agent_num < agents; ++agent_num) {
                        if (agent_num == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[agent_num];
                        if (!(stream >> val && val >= 0 && val < height)) {
                            std::cout << "Warning! Invalid value of '" << CNS_TAG_FINY
                                      << "' tag encountered (or could not convert to integer)" << std::endl;
                            std::cout << "Value of '" << CNS_TAG_FINY << "' tag should be an integer AND >=0 AND < '"
                                      << CNS_TAG_HEIGHT << "' value, which is " << height << std::endl;
                            return false;
                        }
                        goal_j[agent_num] = val;
                    }

                if (agent_num != agents) {
                    std::cout << "Invalid number of '" << CNS_TAG_FINY << "' arguments" << std::endl;
                    return false;
                }

                hasFINY = true;
            }
        }
        else if (value == CNS_TAG_GRID) {
            hasGrid = true;
            if (!(hasHeight && hasWidth)) {
                std::cout << "Error! No '" << CNS_TAG_WIDTH << "' tag or '" << CNS_TAG_HEIGHT << "' tag before '"
                          << CNS_TAG_GRID << "'tag encountered!" << std::endl;
                return false;
            }
            element = mapnode->FirstChildElement();
            while (grid_i < height) {
                if (!element) {
                    std::cout << "Error! Not enough '" << CNS_TAG_ROW << "' tags inside '" << CNS_TAG_GRID << "' tag."
                              << std::endl;
                    std::cout << "Number of '" << CNS_TAG_ROW
                              << "' tags should be equal (or greater) than the value of '" << CNS_TAG_HEIGHT
                              << "' tag which is " << height << std::endl;
                    return false;
                }
                std::string str = element->GetText();
                std::vector<std::string> elems;
                std::stringstream ss(str);
                std::string item;
                while (std::getline(ss, item, ' '))
                    elems.push_back(item);
                rowiter = grid_j = 0;
                int val;
                if (elems.size() > 0)
                    for (grid_j = 0; grid_j < width; ++grid_j) {
                        if (grid_j == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[grid_j];
                        stream >> val;
                        Grid[grid_i][grid_j] = val;
                    }

                if (grid_j != width) {
                    std::cout << "Invalid value on " << CNS_TAG_GRID << " in the " << grid_i + 1 << " " << CNS_TAG_ROW
                              << std::endl;
                    return false;
                }
                ++grid_i;

                element = element->NextSiblingElement();
            }
        }
        else if (value == CNS_TAG_AGENTS) {
            if (hasAgents) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_AGENTS << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_AGENTS << "' =" << cellSize << "will be used."
                          << std::endl;
            }
            else {
                if (!((stream >> agents) && (agents > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_AGENTS
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_AGENTS << "' tag should be an integer AND >0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_AGENTS
                              << "' tag will be encountered later..." << std::endl;

                }
                else
                    hasAgents = true;
            }
        }
    }
    //some additional checks
    if (!hasGrid) {
        std::cout << "Error! There is no tag 'grid' in xml-file!\n";
        return false;
    }

    if (!hasAgents)
        return false;

    if (!(hasFINX && hasFINY && hasSTX && hasSTY))
        return false;

    for (int ind = 0; ind < agents; ++ind)
        if (Grid[start_i[ind]][start_j[ind]] != CN_GC_NOOBS) {
            std::cout << "Error! Start cell is not traversable (cell's value is" << Grid[start_i[ind]][start_j[ind]] << ")!"
                      << std::endl;
            return false;
        }

    for (int ind = 0; ind < agents; ++ind)
        if (Grid[goal_i[ind]][goal_j[ind]] != CN_GC_NOOBS) {
            std::cout << "Error! Goal cell is not traversable (cell's value is" << Grid[goal_i[ind]][goal_j[ind]] << ")!"
                    << std::endl;
            return false;
        }

    return true;
}



int Map::getValue(int i, int j) const
{
    if (i < 0 || i >= height)
        return -1;

    if (j < 0 || j >= width)
        return -1;

    return Grid[i][j];
}

int Map::getMapHeight() const
{
      return height;
}

int Map::getMapWidth() const
{
      return width;
}

double Map::getCellSize() const
{
      return cellSize;
}

int Map::getStartX(int agent_number) const
{
    return start_i[agent_number];
}

int Map::getStartY(int agent_number) const
{
    return start_j[agent_number];
}

int Map::getGoalX(int agent_number) const
{
    return goal_i[agent_number];
}

int Map::getGoalY(int agent_number) const
{
    return goal_j[agent_number];
}

int Map::getAgents() const
{
    return agents;
}
