#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include "GameConstants.h"
#include <algorithm>

class MetaFileHandler
{
    static std::fstream metadataFileHandle;
    static std::string metadataFile;
    static std::vector<std::pair<uint32_t,std::string>> savedHighScoreData;
    static unsigned int minHighScore;

    public:
    MetaFileHandler()
    {}

    static unsigned int getMinHigScore();

    static void updateNewHighScore(unsigned int score, const std::string& username);

    static void readMetaDataFile();

    static void saveMetaDataFileAndClose();

    static std::string getHighScoreDisplayString();    
};


