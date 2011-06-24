#include "file.h"

File::File(QString fileName)
{
    this->fileName = fileName + ".mp4";
    this->tempFileName1 = fileName + "1.mp4";
    this->tempFileName2 = fileName + "2.mp4";

    init();
}

/*
  If not exists, creates app catalog (where are stored temporary files)
  */
void File::createAppCatalog()
{
    QDir dir("/home/user/." APP_NAME);

    if(!dir.exists()) {
        dir.mkdir("/home/user/." APP_NAME);
    }
}

bool File::init()
{
    if(checkIfNotExists())
        return false;

    //activeFile = tempFileName1
    activeFile = 1;

    return true;
}

bool File::checkIfNotExists()
{
    createAppCatalog();

    QFile file1("/home/user/." APP_NAME "/" + tempFileName1);
    QFile file2("/home/user/." APP_NAME "/" + tempFileName2);

    if(file1.exists() || file2.exists())
        return false;

    return true;
}

/*
  Changes currenlty use temporary file
  */
void File::changeFile()
{
    if(activeFile) {
        QFile file("/home/user/." APP_NAME "/" + tempFileName2);

        activeFile = 2;
        file.remove();

    } else {
        QFile file("/home/user/." APP_NAME "/" + tempFileName1);

        activeFile = 1;
        file.remove();
    }
}

/*
  Moves currently recorder part to .videos dir
  */
bool File::fileReady()
{
    bool succeed = false;

    if(activeFile) {
        QFile file("/home/user/." APP_NAME "/" + tempFileName1);

        succeed = file.rename("/home/user/MyDocs/.videos/" + fileName);
    } else {
        QFile file("/home/user/." APP_NAME "/" + tempFileName2);

        succeed = file.rename("/home/user/MyDocs/.videos/" + fileName);
    }

    removeTempFiles();

    return succeed;
}

/*
  Removes temporary files
  */
void File::removeTempFiles()
{
    QFile file1("/home/user/." APP_NAME "/" + tempFileName1);
    QFile file2("/home/user/." APP_NAME "/" + tempFileName2);

    file1.remove();
    file2.remove();
}

/*
  Returns number of the currently used temporary file
  */
int File::getActiveFileNumber()
{
    return activeFile;
}

/*
  Returns full name of the currently used temporary file
  */
QString File::getActiveFile()
{
    if(activeFile)
        return QString("/home/user/." APP_NAME "/" + tempFileName1);
    else
        return QString("/home/user/." APP_NAME "/" + tempFileName2);
}
