#pragma once

#include <QtGlobal>

class FileShredder
{
public:
    static bool shredFile(const QString& filePath,
                          int passes = 1,
                          qint64 chunkSize = 10 * 1024 * 1024);

private:
    static QByteArray generateRandomData(qint64 size);
};
