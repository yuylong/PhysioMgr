#ifndef PSMDLGPHYSIOSEL_H
#define PSMDLGPHYSIOSEL_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgPhysioSel;
}

class PsmDlgPhysioSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPhysioSel(QWidget *parent = 0);
    ~PsmDlgPhysioSel();

    PsmService *getService() const;
    void setService(PsmService *service);

    bool getSelectedPhysioItem(PsmSrvPhysioItem *physio);

private slots:
    void on_pbRefresh_clicked();
    void on_leCond_returnPressed();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::PsmDlgPhysioSel *ui;
    PsmService *service;

    bool keyPressedInLe;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // PSMDLGPHYSIOSEL_H
