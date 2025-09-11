#include "MetaFileHandler.h"

std::fstream MetaFileHandler::metadataFileHandle;
std::string MetaFileHandler::metadataFile;
std::vector<std::pair<uint32_t,std::string>> MetaFileHandler::savedHighScoreData;
unsigned int MetaFileHandler::minHighScore = 0;


unsigned int MetaFileHandler::getMinHighScore()
{
    return minHighScore;
}

void MetaFileHandler::updateNewHighScore(unsigned int score, const std::string& username)
{
    if(score > minHighScore)
    {
        bool usenameExist = false;
        for (auto it = savedHighScoreData.begin(); it != savedHighScoreData.end(); it++)
        {
            // keeping second condition comparison >= in case this function is called twice by accident
            if (it->second == username && score >= it->first)
            {
                it->first = score;
                usenameExist = true;
                break;
            }
        }

        if(!usenameExist)
        {
            savedHighScoreData.push_back(std::make_pair(score, username));
        }

        std::sort(savedHighScoreData.begin(), savedHighScoreData.end(),
                    [](const auto &a, const auto &b) { return a.first > b.first; });

        if (savedHighScoreData.size() > NO_SAVED_HIGHSCORE_USERNAMES)
        {
            savedHighScoreData.erase(std::prev(savedHighScoreData.end()));
        }

        minHighScore = savedHighScoreData.back().first;

    }
}

void MetaFileHandler::readMetaDataFile()
{
    metadataFile =
        std::getenv("HOME") + std::string(TOSTRINGYFY(META_DATA_FILE_PATH)) + std::string(META_DATA_FILE_NAME);
    bool fileexist = std::filesystem::exists(metadataFile);
    if (fileexist)
    {
        metadataFileHandle.open(metadataFile, std::ios::in | std::ios::binary);
        if (metadataFileHandle.is_open())
        {
            // Read username until comma
            std::string username;
            uint8_t highscorebuffer[4];
            char ch;
            while (metadataFileHandle.read(&ch, sizeof(ch)))
            {
                if (ch == ',')
                {
                    uint8_t highscorebuffer[4];
                    uint32_t tempHighScore = 0;
                    metadataFileHandle.read((char *)(&highscorebuffer[0]), sizeof(highscorebuffer));

                    for (unsigned int i = 0; i < 4; i++)
                    {
                        tempHighScore |= (highscorebuffer[i] << (i * 8));
                    }
                    savedHighScoreData.push_back(std::make_pair(tempHighScore, username));

                    if(minHighScore == 0)
                    {
                        minHighScore = tempHighScore;
                    }
                    else
                    {
                        minHighScore = std::min(tempHighScore, minHighScore);
                    }

                    std::cout << "read from file:" << username << tempHighScore << std::endl;
                    username.clear();
                }
                else if (ch == ';')
                {
                    continue;
                }
                else
                {
                    username += ch;
                }
            }
        }

        metadataFileHandle.close();
        metadataFileHandle.open(metadataFile, std::ios::out | std::ios::binary);
    }
    else
    {
        auto dirPath = std::getenv("HOME") + std::string(TOSTRINGYFY(META_DATA_FILE_PATH));
        if (mkdir(dirPath.c_str(), 0755) != 0 && errno != EEXIST) {
            std::cerr << "Failed to create directory: " << dirPath << " - " << strerror(errno) << std::endl;
        }

        metadataFileHandle.open(metadataFile, std::ios::out | std::ios::binary);
        if (metadataFileHandle.is_open())
            std::cout << "file is created and opened" << std::endl;
    }

}

void MetaFileHandler::saveMetaDataFileAndClose()
{
    char comma = ',';
    char semicolon = ';';

    if (!metadataFileHandle.is_open())
    {
        return;
    }

    for (auto &[score, name] : savedHighScoreData)
    {
        std::cout << "writing to file:" << score << name << std::endl;
        metadataFileHandle.write(name.c_str(), name.size());
        metadataFileHandle.write(&comma, sizeof(comma));
        metadataFileHandle.write((char *)(&score), 4);
        metadataFileHandle.write(&semicolon, sizeof(semicolon));
    }
    metadataFileHandle.close();
}


std::string MetaFileHandler::getHighScoreDisplayString()
{
    std::string highScoreDisplayData;

    if (savedHighScoreData.size() > 0)
    {
        // displaying in descending order
        std::sort(savedHighScoreData.begin(), savedHighScoreData.end(),
                    [](const auto &a, const auto &b) { return a.first > b.first; });
        for (auto &[score, username] : savedHighScoreData)
        {
            highScoreDisplayData += username;
            highScoreDisplayData += " : ";
            highScoreDisplayData += std::to_string(score);
            highScoreDisplayData += "\n";
        }
    }

    return highScoreDisplayData;
}