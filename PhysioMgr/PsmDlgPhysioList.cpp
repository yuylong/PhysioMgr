#include "PsmDlgPhysioList.h"
#include "ui_PsmDlgPhysioList.h"

PsmDlgPhysioList::PsmDlgPhysioList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioList)
{
    ui->setupUi(this);
}

PsmDlgPhysioList::~PsmDlgPhysioList()
{
    delete ui;
}
