#include "device.h"

Device::Device()
{

}

QStringList Device::listDevices()
{
    QStringList result;
#ifdef Q_OS_LINUX
    QDir blockDevicesDir(SYS_BLOCK_DEV_DIR, BLOCK_DEV_MASK);
    QStringList devices = blockDevicesDir.entryList();
    for (auto device = devices.begin(); device < devices.end(); device++) {
        QFile modelFile(QString(DEV_MODEL_PATH).arg(*device));
        if (modelFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString model(modelFile.readLine());
            if (model.contains(DEV_MODEL)) {
                result << QString(DEV_PATH).arg(*device);
            }
        }
    }
#endif
#ifdef Q_OS_WIN32
    unsigned long driveMask = ::GetLogicalDrives();
    ULONG id;
        char letter = 'A';
        while (driveMask != 0) {
            if (driveMask & 1) {
                QString path = QString(DEV_PATH).arg(letter);
                wchar_t wpath[MAX_PATH];
                path.toWCharArray(wpath);
                std::wstring wspath(wpath);
                result << path;
            }
            driveMask >>= 1;
            letter++;
        }
#endif
    return result;
}
