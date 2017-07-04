#include "PsmService.h"

#include <QList>
#include <QApplication>

PsmService::PsmService() :
    configfile(), database()
{
}

QString PsmService::getConfigFile() const
{
    return this->configfile;
}

void PsmService::setConfigFile(const QString &configfile)
{
    this->configfile = configfile;
}

QMainWindow *PsmService::getParentWindow() const
{
    return this->parent;
}

void PsmService::setParentWindow(QMainWindow *window)
{
    this->parent = window;
}

const PsmDatabase *PsmService::getDatabase() const
{
    return &this->database;
}

PsmDatabase *PsmService::getDatabase()
{
    return &this->database;
}

bool PsmService::init(QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    if (this->configfile.isEmpty()) {
        QMessageBox::critical(window, "致命错误", "数据库配置文件未设置，请联系开发人员解决！");
        return false;
    }


    this->database.configConnectionFromFile(this->configfile);
    if ( !this->database.isConnectionConfigured() ) {
        QMessageBox::critical(window, "致命错误", "无法打开数据库配置文件，请联系开发人员解决！");
         return false;
    }

    this->database.startDatabase();
    if (!this->database.isDatabaseStarted()) {
        QMessageBox::critical(window, "致命错误", "数据库无法完成配置工作，请联系开发人员解决！");
        return false;
    }

    return true;
}

int PsmService::getTableSelectedRow(QTableWidget *tbl)
{
    QList<QTableWidgetSelectionRange> idxlist = tbl->selectedRanges();
    if (idxlist.count() < 1)
        return -1;

    const QTableWidgetSelectionRange &idx = idxlist.at(0);
    return idx.topRow();
}

bool PsmService::readSelectedDepartment(QTableWidget *tbl, PsmSrvDepartment *dep)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    if (itemid == NULL || itemname == NULL)
        return false;

    dep->id = itemid->text();
    dep->name = itemname->text();
    return true;
}

QString PsmService::readSelectedDepartmentId(QTableWidget *tbl)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return QString();

    QTableWidgetItem *item = tbl->item(rowidx, 0);
    if (item == NULL)
        return QString();

    return item->text();
}

void PsmService::refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM departments;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成科室列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
}

void PsmService::insertNewDepartment(const PsmSrvDepartment &dep, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO departments VALUES(?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, dep.id);
    query.bindValue(1, dep.name);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的科室。" + exterrstr);
    return;
}

void PsmService::updateDepartment(const PsmSrvDepartment &dep, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE departments SET name=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, dep.name);
    query.bindValue(1, dep.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成科室信息更新。" + exterrstr);
    return;
}

void PsmService::deleteDepartment(const QString &depid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM departments WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, depid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定科室。" + exterrstr);
    return;
}

void PsmService::refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM physio_items;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成理疗项目列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
}
