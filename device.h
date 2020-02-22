#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#ifdef Q_OS_LINUX
#include <QDir>
#endif
#ifdef Q_OS_WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#ifdef Q_OS_LINUX
#define SYS_BLOCK_DEV_DIR "/sys/block"
#define BLOCK_DEV_MASK "sd?"
#define DEV_MODEL_PATH "/sys/block/%1/device/model"
#define DEV_PATH "/dev/%1"
#endif
#ifdef Q_OS_WIN32
#define DEV_PATH "\\\\.\\%1:\\"
#endif
#define DEV_MODEL "ANHOT_CHEAP_POI"

class Device
{
public:
    Device();
    static QStringList listDevices();
};

#endif // DEVICE_H
