#include "fileshredder.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QRandomGenerator>

bool FileShredder::shredFile(const QString& filePath, int passes, qint64 chunkSize)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    qint64 fileSize = file.size();
    qint64 remainingBytes;

    qWarning() << "Starting to shred file:" << filePath;
    qWarning() << "File size:" << fileSize << "bytes";
    qWarning() << "Number of passes:" << passes;

    for (int pass = 1; pass <= passes; ++pass) {
        qWarning() << "Starting pass" << pass << "of" << passes;
        file.seek(0);
        remainingBytes = fileSize;
        qint64 processedBytes = 0;

        while (remainingBytes > 0) {
            qint64 currentChunkSize = qMin(chunkSize, remainingBytes);

            // Overwrite with random data
            QByteArray randomData = generateRandomData(currentChunkSize);
            if (file.write(randomData) != currentChunkSize) {
                qWarning() << "Error writing random data in pass" << pass;
                file.close();
                return false;
            }
            file.flush();

            // Overwrite with zeros
            QByteArray zeroData(currentChunkSize, 0);
            file.seek(file.pos() - currentChunkSize);
            if (file.write(zeroData) != currentChunkSize) {
                qWarning() << "Error writing zero data in pass" << pass;
                file.close();
                return false;
            }

            file.flush();
            remainingBytes -= currentChunkSize;
            processedBytes += currentChunkSize;

            // Print progress
            int progressPercent = static_cast<int>(
                (static_cast<double>(processedBytes) / fileSize) * 100);
            qWarning() << "Pass" << pass << "progress:" << progressPercent << "%";
        }

        qWarning() << "Completed pass" << pass;
    }

    file.close();

    qWarning() << "File shredding complete. Attempting to delete the file.";

    // Delete the file
    if (QFile::remove(filePath)) {
        qWarning() << "File successfully deleted.";
        return true;
    } else {
        qWarning() << "Failed to delete the file.";
        return false;
    }
    return true;
}

QByteArray FileShredder::generateRandomData(qint64 size)
{
    QByteArray data;
    data.reserve(size);
    for (qint64 i = 0; i < size; ++i) {
        data.append(static_cast<char>(QRandomGenerator::global()->generate() % 256));
    }
    return data;
}
