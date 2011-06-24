#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QString>
#include <QDir>

#define APP_NAME "camdrive"

class File
{
public:
    File(QString fileName);
    bool init();
    bool fileReady();
    void changeFile();
    int getActiveFileNumber();
    QString getActiveFile();

private:
    void createAppCatalog();
    bool checkIfNotExists();
    void removeTempFiles();

    QString fileName;
    QString tempFileName1;
    QString tempFileName2;
    int activeFile;
};

#endif // FILE_H
