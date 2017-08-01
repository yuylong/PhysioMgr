#include "PsmDlgDepartment.h"
#include "ui_PsmDlgDepartment.h"

#include <QKeyEvent>

PsmDlgDepartment::PsmDlgDepartment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDepartment)
{
    ui->setupUi(this);

    this->installEventFilter(this);
}

PsmDlgDepartment::~PsmDlgDepartment()
{
    delete ui;
}

bool PsmDlgDepartment::eventFilter(QObject *obj, QEvent *event)
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

QString PsmDlgDepartment::getDepartID() const
{
    return ui->leDepartID->text();
}

QString PsmDlgDepartment::getDepartName() const
{
    return ui->leDepartName->text();
}

void PsmDlgDepartment::setDepartID(const QString &id)
{
    ui->leDepartID->setText(id);
}

void PsmDlgDepartment::setDepartName(const QString &name)
{
    ui->leDepartName->setText(name);
}

void PsmDlgDepartment::lockDepartID()
{
    ui->leDepartID->setEnabled(false);
}

void PsmDlgDepartment::unlockDepartID()
{
    ui->leDepartID->setEnabled(true);
}

void PsmDlgDepartment::on_leDepartID_returnPressed()
{
    ui->leDepartName->setFocus();
}

void PsmDlgDepartment::on_leDepartName_returnPressed()
{
    emit (ui->buttonBox->accepted());
}
