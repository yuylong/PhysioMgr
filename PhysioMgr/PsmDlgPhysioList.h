#ifndef PSMDLGPHYSIOLIST_H
#define PSMDLGPHYSIOLIST_H

#include <QDialog>

namespace Ui {
class PsmDlgPhysioList;
}

class PsmDlgPhysioList : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPhysioList(QWidget *parent = 0);
    ~PsmDlgPhysioList();

private:
    Ui::PsmDlgPhysioList *ui;
};

#endif // PSMDLGPHYSIOLIST_H
