#ifndef PSMDLGDEPARTSEL_H
#define PSMDLGDEPARTSEL_H

#include <QDialog>

namespace Ui {
class PsmDlgDepartSel;
}

class PsmDlgDepartSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgDepartSel(QWidget *parent = 0);
    ~PsmDlgDepartSel();

private:
    Ui::PsmDlgDepartSel *ui;
};

#endif // PSMDLGDEPARTSEL_H
