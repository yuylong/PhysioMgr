#include "PsmDlgPhysioItem.h"
#include "ui_PsmDlgPhysioItem.h"

#include <QKeyEvent>

PsmDlgPhysioItem::PsmDlgPhysioItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioItem)
{
    ui->setupUi(this);
    ui->lePhysioId->setFocus();

    this->installEventFilter(this);
}

PsmDlgPhysioItem::~PsmDlgPhysioItem()
{
    delete ui;
}

bool PsmDlgPhysioItem::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == this ) {
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return )
                return true;
        }
    }

    return QDialog::eventFilter(obj, event);
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

void PsmDlgPhysioItem::on_lePhysioId_returnPressed()
{
    ui->lePhysioName->setFocus();
}

void PsmDlgPhysioItem::on_lePhysioName_returnPressed()
{
    ui->dsbPhysioPrice->setFocus();
}
