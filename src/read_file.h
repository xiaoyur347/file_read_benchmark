#ifndef READ_FILE_H
#define READ_FILE_H

#include <fstream>
#include <string>

typedef void (*DealFile)(const std::string &strLine, void *arg);

static void ReadFileFunc(const char *szFileName, DealFile func, void *arg)
{
    std::ifstream fin;
    fin.open(szFileName);

    if (!fin)
    {
        return;
    }

    std::string strLine;
    bool bFirstLine = true;

    while (getline(fin, strLine))
    {
        /** BOM */
        if (bFirstLine)
        {
            bFirstLine = false;
            if (strLine.compare(0,3,"\xEF\xBB\xBF")==0)
            {
                strLine.erase(0, 3);
            }
        }

        std::string::size_type index = strLine.find_last_not_of("\r\n");

        if (index == std::string::npos)
        {
            continue;
        }

        strLine.erase(strLine.begin() + index + 1, strLine.end());

        //Deal
        (func)(strLine, arg);
    }

    fin.close();
}

#endif