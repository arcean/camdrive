#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    connect(ui->z1OK, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->z2OK, SIGNAL(clicked()), this, SLOT(hide()));

}

Settings::~Settings()
{
    delete ui;
}
