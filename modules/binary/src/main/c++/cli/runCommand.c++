#include "runCommand.h++"

#include "configuration.h++"
#include "../cache/diskBackedCache.h++"
#include <iomanip>

namespace TyrantMutator {
    namespace CLI {

        RunCommand::RunCommand(Configuration configuration
                              )
        : Command(configuration)
        {
            // the core
            C::SimulatorCore::Ptr simulator = configuration.constructCore();
            // the cache
            Cache::DiskBackedCache::Ptr cache = Cache::DiskBackedCache::Ptr(
                new Cache::DiskBackedCache(simulator)
            );

            this->simulator = cache;
        }

        RunCommand::~RunCommand()
        {
        }

        int RunCommand::execute() {
            C::SimulationResult r = this->simulator->simulate(this->task);
            std::cout << "                                 " << std::setw(11) << "Count"
                      << " " << std::setw(10) << "avg." << std::endl;
            std::cout << "Games played:                    " << std::setw(11) << r.numberOfGames
                      << std::endl;
            std::cout << "Games won:                       " << std::setw(11) << r.gamesWon
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getWinRate() << std::endl;
            std::cout << "Games lost:                      " << std::setw(11) << r.gamesLost
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getLossRate() << std::endl;
            std::cout << "Games stalled:                   " << std::setw(11) << r.gamesStalled
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getStallRate() << std::endl;
            std::cout << "Points for attacker on manual: " << std::setw(13) << r.pointsAttacker
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getManualANPAttacker() << std::endl;
            std::cout << "Points for attacker on auto:   " << std::setw(13) << r.pointsAttackerAuto
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getAutoANPAttacker() << std::endl;
            std::cout << "Points for defender on manual: " << std::setw(13) << r.pointsDefender
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getManualANPDefender() << std::endl;
            std::cout << "Points for defender on auto:   " << std::setw(13) << r.pointsDefenderAuto
                      << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.getAutoANPDefender() << std::endl;
            return 0;
        }

        void
        RunCommand::abort()
        {
            this->simulator->abort();
        }

        void
        RunCommand::setCacheRead(bool cacheRead)
        {
            this->simulator->setReadFromCache(cacheRead);
        }

        void
        RunCommand::setCacheWrite(bool cacheWrite)
        {
            this->simulator->setWriteToCache(cacheWrite);
        }

    }
}
