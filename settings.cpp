#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
}

Settings::~Settings()
{
    delete ui;
}
