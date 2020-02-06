#include "Core/EventLogger.hpp"
#include <QDateTime>
#include <QStandardPaths>
#include <QSysInfo>
#include <QVector>
#include <iostream>
#include <generated/version.hpp>

namespace Core
{

QFile Log::logFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + LOG_FILE_NAME);
bool Log::shouldWriteToStderr;

void Log::e(QString head, QString body)
{
    logWithPriority("[ERR]", head, body);
}
void Log::w(QString head, QString body)
{
    logWithPriority("[WARN]", head, body);
}
void Log::wtf(QString head, QString body)
{
    logWithPriority("[WTF]", head, body);
}
void Log::i(QString head, QString body)
{
    logWithPriority("[INFO]", head, body);
}

void Log::init(bool dumptoStderr)
{
    shouldWriteToStderr = false;
    logFile.open(QIODevice::WriteOnly | QFile::Text);
    if (logFile.isOpen())
    {

        i("Logger", "Event logger has arrived!");
        i("SysInfo", "Gathering system information");
        i("SysInfo", platformInformation());
    }
    else
    {
        std::cerr << ("Failed to start the event logger. Application cannot create log file at " +
                      QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + LOG_FILE_NAME)
                         .toStdString();

        shouldWriteToStderr = true; // Since log to file cannot be performed, log on stderr explicitly
    }
}

QString Log::dateTimeStamp()
{
    return "[" + QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + "]";
}

QString Log::platformInformation()
{
    QString res;
    // Check https://doc.qt.io/qt-5/qsysinfo.html to know what each identifier means
    res.append("buildABI : " + QSysInfo::buildAbi() + "\n");
    res.append("buildCPUArchitecture : " + QSysInfo::buildCpuArchitecture() + "\n");
    res.append("currentCPUArchitecture : " + QSysInfo::currentCpuArchitecture() + "\n");
    res.append("kernelType : " + QSysInfo::kernelType() + "\n");
    res.append("kernelVersion : " + QSysInfo::kernelVersion() + "\n");
    res.append("prettyProductName : " + QSysInfo::prettyProductName() + "\n");
    res.append("machineHostName : " + QSysInfo::machineHostName() + "\n");
    res.append("machineUniqueId : " + QSysInfo::machineUniqueId() + "\n");
    res.append("productType : " + QSysInfo::productType() + "\n");
    res.append("productVersion : " + QSysInfo::productVersion() + "\n");
    res.append("Appversion : " + QString::fromStdString(APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_VERSION_PATCH));
    return res;
}

void Log::logWithPriority(QString priority, QString head, QString body)
{
    QString logContent;
    logContent.append(dateTimeStamp());
    logContent.append(priority);
    logContent.append("[" + head + "]");
    logContent.append(body);
    logContent.append("\n");
    if (logFile.isWritable())
    {
        logFile.write(logContent.toLocal8Bit());
        logFile.flush();
    }
    if (shouldWriteToStderr)
        std::cerr << logContent.toStdString();
}

} // namespace Core