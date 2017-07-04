#include "PsmDlgPhysioItem.h"
#include "ui_PsmDlgPhysioItem.h"

PsmDlgPhysioItem::PsmDlgPhysioItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioItem)
{
    ui->setupUi(this);
}

PsmDlgPhysioItem::~PsmDlgPhysioItem()
{
    delete ui;
}

QString PsmDlgPhysioItem::getPhysioId() const
{
    return ui->lePhysioId->text();
}

QString PsmDlgPhysioItem::getPhysioName() const
{
    return ui->lePhysioName->text();
}

double PsmDlgPhysioItem::getPhysioPrice() const
{
    return ui->dsbPhysioPrice->value();
}

void PsmDlgPhysioItem::setPhysioId(const QString &id)
{
    ui->lePhysioId->setText(id);
}

void PsmDlgPhysioItem::setPhysioName(const QString &name)
{
    ui->lePhysioName->setText(name);
}

void PsmDlgPhysioItem::setPhysioPrice(double price)
{
    ui->dsbPhysioPrice->setValue(price);
}

void PsmDlgPhysioItem::lockPhysioId()
{
    ui->lePhysioId->setEnabled(false);
}

void PsmDlgPhysioItem::unlockPhysioId()
{
    ui->lePhysioId->setEnabled(true);
}
