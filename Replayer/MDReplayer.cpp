#include "MDReplayer.h"

MDReplayer::MDReplayer(const std::string& dataPath, const std::string& code)
    : csvPath(dataPath + "/" + code + ".csv")
{
    
}

void MDReplayer::Subscribe(std::shared_ptr<MDSubscriber> sub)
{
    MDSubs.push_back(sub);
}

bool MDReplayer::Init()
{
    bool res = loader.LoadFile(csvPath);
    if (!res) return res;
    for (const auto subs : MDSubs)
    {
        subs->OnBacktestInit();
    }
    return res;
}

void MDReplayer::Run()
{
    for (const auto subs : MDSubs)
    {
        subs->OnBacktestStart();
    }
    while (loader.NextRow())
    {
        auto data = loader.ToDailyData();
        for (const auto subs : MDSubs)
        {
            subs->OnMDUpdate(data);
        }
    }
    for (const auto subs : MDSubs)
    {
        subs->OnBacktestEnd();
    }
}