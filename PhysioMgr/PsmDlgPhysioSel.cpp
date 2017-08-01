#include "PsmDlgPhysioSel.h"
#include "ui_PsmDlgPhysioSel.h"

PsmDlgPhysioSel::PsmDlgPhysioSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioSel)
{
    ui->setupUi(this);
    this->service = NULL;
    this->keyPressedInLe = false;

    this->installEventFilter(this);
    ui->leCond->setFocus();
}

PsmDlgPhysioSel::~PsmDlgPhysioSel()
{
    delete ui;
}

bool PsmDlgPhysioSel::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == this ) {
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( this->keyPressedInLe &&
                 (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) ) {
                this->keyPressedInLe = false;
                return true;
            }
        }
    }

    return QDialog::eventFilter(obj, event);
}

PsmService *PsmDlgPhysioSel::getService() const
{
    return this->service;
}

void PsmDlgPhysioSel::setService(PsmService *service)
{
    this->service = service;
}

bool PsmDlgPhysioSel::getSelectedPhysioItem(PsmSrvPhysioItem *physio)
{
    if (this->service == NULL || physio == NULL)
        return false;

    return this->service->readSelectedPhysioItem(ui->tableWidget, physio);
}

void PsmDlgPhysioSel::on_pbRefresh_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshPhysioItemList(ui->lblCount, ui->tableWidget, this);
    else
        this->service->searchPhysioItem(ui->leCond->text(), ui->lblCount, ui->tableWidget, this);

    ui->leCond->selectAll();
    if ( ui->tableWidget->rowCount() > 0 ) {
        ui->tableWidget->selectRow(0);
        ui->tableWidget->setFocus();
    } else {
        ui->leCond->setFocus();
    }
}

void PsmDlgPhysioSel::on_leCond_returnPressed()
{
    this->keyPressedInLe = true;
    this->on_pbRefresh_clicked();
}

void PsmDlgPhysioSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
