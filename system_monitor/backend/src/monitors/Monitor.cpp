#include "Monitor.hpp"
#include <unistd.h> 
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include <filesystem>
#include <sys/statvfs.h>
namespace {


string formatUptime(long totalSeconds)
{
    long days = totalSeconds / 86400;
    totalSeconds %= 86400;

    long hours = totalSeconds / 3600;
    totalSeconds %= 3600;

    long minutes = totalSeconds / 60;

    ostringstream oss;

    if (days > 0) {
        oss << days << "d ";
    }

    oss << hours << "h " << minutes << "m";

    return oss.str();
}

}

void readCpuLine(istream& file, vector<unsigned long long>& values)
{
    string cpuLabel;
    file >> cpuLabel;

    unsigned long long user, nice, system, idle,
        iowait, irq, softirq, steal, guest, guestNice;

    file >> user >> nice >> system >> idle
         >> iowait >> irq >> softirq >> steal
         >> guest >> guestNice;

    unsigned long long total =
        user + nice + system + idle +
        iowait + irq + softirq + steal;

    unsigned long long idleAll = idle + iowait;

    values = {total, idleAll};
}

SystemInfo Monitor::collect()
{
    SystemInfo info;

    {
        ifstream file("/proc/stat");
        
        if (file.is_open()) {
            string line;
            vector<unsigned long long> current(2);
            getline(file, line);
            istringstream ss1(line);
            readCpuLine(ss1, current);

            unsigned long long total1 = current[0];
            unsigned long long idle1 = current[1];

            this_thread::sleep_for(chrono::milliseconds(100));
            file.clear();
            file.seekg(0);
            getline(file, line);
            istringstream ss2(line);
            readCpuLine(ss2, current);

            unsigned long long total2 = current[0];
            unsigned long long idle2 = current[1];

            unsigned long long totalDiff = total2 - total1;
            unsigned long long idleDiff = idle2 - idle1;

            if (totalDiff > 0) {
                info.cpu.totalUsage = (1.0 - static_cast<double>(idleDiff) / static_cast<double>(totalDiff)) * 100.0;
            } 
            else {
                info.cpu.totalUsage = 0.0;
            }
        info.cpu.perCoreUsage.clear();
        }
        else {
            info.cpu.totalUsage = 0.0;
            info.cpu.perCoreUsage.clear();
        }
    }

    {
        char buffer[256]{};
        if (gethostname(buffer, sizeof(buffer)) == 0) {
            info.system.hostname = buffer;
        } 
        else {
            info.system.hostname = "unknown";
        }
    }

    {
        ifstream file("/proc/uptime");

        if (file.is_open()) {
            double uptimeSeconds = 0.0;
            file >> uptimeSeconds;

            info.system.uptime =
                formatUptime(static_cast<long>(uptimeSeconds));
        } 
        else {
            info.system.uptime = "unknown";
        }
    }

    {
        ifstream file("/proc/meminfo");

        if (file.is_open()) {
            string key;
            uint64_t value = 0;
            string unit;

            uint64_t memTotalKB = 0;
            uint64_t memAvailableKB = 0;

            while (file >> key >> value >> unit) {
                if (key == "MemTotal:") {
                    memTotalKB = value;
                }
                else if (key == "MemAvailable:") {
                    memAvailableKB = value;
                }
                if (memTotalKB != 0 && memAvailableKB != 0) {
                    break;
                }
            }

            if (memTotalKB > 0) {
                uint64_t usedKB = memTotalKB - memAvailableKB;
                info.memory.totalMB = memTotalKB / 1024;
                info.memory.usedMB = usedKB / 1024;
                info.memory.freeMB = memAvailableKB / 1024;
                info.memory.usagePercent = static_cast<double>(usedKB) * 100.0 /static_cast<double>(memTotalKB);
            }
            else {
                info.memory.totalMB = 0;
                info.memory.usedMB = 0;
                info.memory.freeMB = 0;
                info.memory.usagePercent = 0.0;
            }
        }
        else {
            info.memory.totalMB = 0;
            info.memory.usedMB = 0;
            info.memory.freeMB = 0;
            info.memory.usagePercent = 0.0;
        }
    }
    

    struct statvfs stat;
    
    if (statvfs("/", &stat) == 0) {
        uint64_t totalBytes = stat.f_blocks * stat.f_frsize;
        uint64_t availBytes = stat.f_bavail * stat.f_frsize;
        uint64_t freeBytes  = stat.f_bfree * stat.f_frsize;
        uint64_t usedBytes = totalBytes - freeBytes;
        
        constexpr double GB = 1024.0 * 1024.0 * 1024.0;

        info.disk.totalGB = totalBytes / GB;
        info.disk.freeGB  = availBytes / GB;
        info.disk.usedGB  = usedBytes / GB;
        
        if (totalBytes > 0) {
            info.disk.usagePercent = (double)usedBytes * 100.0 / (double)totalBytes;
        } 
        else {
            info.disk.usagePercent = 0.0;
        }
    }
    else {
        info.disk.totalGB = 0.0;
        info.disk.freeGB  = 0.0;
        info.disk.usedGB  = 0.0;
        info.disk.usagePercent = 0.0;
    }

    {
        ifstream file("/proc/net/dev");
        if (file.is_open()) {
            string line;

            getline(file, line);
            getline(file, line);

            uint64_t totalRxBytes = 0;
            uint64_t totalTxBytes = 0;

            while (getline(file, line)) {
                replace(line.begin(), line.end(), ':', ' ');
                istringstream iss(line);
                string interfaceName;
                uint64_t rxBytes = 0;
                uint64_t txBytes = 0;
                uint64_t dummy = 0;

                iss >> interfaceName;
                iss >> rxBytes;

                for (int i = 0; i < 7; ++i) {
                    iss >> dummy;
                }

                iss >> txBytes;

                if (interfaceName != "lo") {
                    totalRxBytes += rxBytes;
                    totalTxBytes += txBytes;
                }
            }

            info.network.rxBytes = totalRxBytes;
            info.network.txBytes = totalTxBytes;
        }
        else {
            info.network.rxBytes = 0;
            info.network.txBytes = 0;
        }
    }
    
    return info;
}