#include "fileshredder.h" // Assume the FileShredder class is in this header
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("FileShredder");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Secure file shredder");
    parser.addHelpOption();
    parser.addVersionOption();

    // Add option for file path
    QCommandLineOption filePathOption(QStringList() << "f"
                                                    << "file",
                                      QCoreApplication::translate("main",
                                                                  "File to shred"),
                                      QCoreApplication::translate("main", "file"));
    parser.addOption(filePathOption);

    // Add option for number of passes
    QCommandLineOption
        passesOption(QStringList() << "p"
                                   << "passes",
                     QCoreApplication::translate("main", "Number of shredding passes"),
                     QCoreApplication::translate("main", "number"),
                     "1");
    parser.addOption(passesOption);

    // Process the command line arguments
    parser.process(a);

    // Get the file path
    QString filePath = parser.value(filePathOption);
    if (filePath.isEmpty()) {
        qWarning() << "Error: File path is required.";
        parser.showHelp(1);
    }

    // Get the number of passes
    bool ok;
    int passes = parser.value(passesOption).toInt(&ok);
    if (!ok || passes <= 0) {
        qWarning() << "Error: Invalid number of passes.";
        parser.showHelp(1);
    }

    // Perform file shredding
    bool success = FileShredder::shredFile(filePath, passes);

    if (success) {
        qWarning() << "File shredding completed successfully.";
        return 0;
    } else {
        qWarning() << "File shredding failed.";
        return 1;
    }
}
