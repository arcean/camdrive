#include <QGraphicsScene>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->authorsButton, SIGNAL(clicked()), this, SLOT(toggleAuthorsPage()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::toggleAuthorsPage()
{
    if(ui->stackedWidget->currentIndex())
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}
