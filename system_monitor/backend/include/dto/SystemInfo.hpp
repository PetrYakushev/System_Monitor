#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using namespace std;

struct SystemOverview {
    string hostname;
    string uptime;
};

struct CpuInfo {
    double totalUsage = 0.0;
    vector<double> perCoreUsage;
};

struct MemoryInfo {
    uint64_t totalMB = 0;
    uint64_t usedMB = 0;
    uint64_t freeMB = 0;
    double usagePercent = 0.0;
};

struct DiskInfo {
    uint64_t totalGB = 0;
    uint64_t usedGB = 0;
    uint64_t freeGB = 0;
    double usagePercent = 0.0;
};

struct NetworkInfo {
    uint64_t rxBytes = 0;
    uint64_t txBytes = 0;
};

struct ProcessInfo {
    int pid = 0;
    string name;
    double cpuPercent = 0.0;
    double memoryPercent = 0.0;
    string state;
};

struct SystemInfo {
    SystemOverview system;
    CpuInfo cpu;
    MemoryInfo memory;
    DiskInfo disk;
    NetworkInfo network;
    vector<ProcessInfo> processes;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SystemOverview, hostname,uptime)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CpuInfo,totalUsage,perCoreUsage)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MemoryInfo, totalMB,usedMB,freeMB,usagePercent)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DiskInfo, totalGB, usedGB,freeGB, usagePercent)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NetworkInfo,rxBytes,txBytes)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProcessInfo,pid,name,cpuPercent,memoryPercent,state)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SystemInfo,system,cpu,memory,disk,network,processes)