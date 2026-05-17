"use strict";
async function loadSystemInfo() {
    try {
        const response = await fetch("/api/system");
        const data = await response.json();
        updateUI(data);
    }
    catch (err) {
        console.error("Failed to load system info:", err);
    }
}
function formatBytes(bytes) {
    const units = ["B", "KB", "MB", "GB", "TB"];
    let value = bytes;
    let unitIndex = 0;
    while (value >= 1024 && unitIndex < units.length - 1) {
        value /= 1024;
        unitIndex++;
    }
    return `${value.toFixed(2)} ${units[unitIndex]}`;
}
function updateUI(data) {
    document.getElementById("hostname").innerText =
        data.system.hostname;
    document.getElementById("uptime").innerText =
        data.system.uptime;
    document.getElementById("cpu").innerText =
        data.cpu.totalUsage.toFixed(2) + "%";
    document.getElementById("ram").innerText =
        `${data.memory.usedMB} / ${data.memory.totalMB} MB (${data.memory.usagePercent.toFixed(2)}%)`;
    document.getElementById("disk").innerText =
        `${data.disk.usedGB.toFixed(2)} GB / ` +
            `${data.disk.totalGB.toFixed(2)} GB ` +
            `(${data.disk.usagePercent.toFixed(2)} %)`;
    document.getElementById("network").innerText =
        `RX: ${formatBytes(data.network?.rxBytes ?? 0)}, ` +
            `TX: ${formatBytes(data.network?.txBytes ?? 0)}`;
}
loadSystemInfo();
setInterval(loadSystemInfo, 1000);
