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

QString PsmService::readTableSelectedId(QTableWidget *tbl, int idx)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return QString();

    QTableWidgetItem *item = tbl->item(rowidx, idx);
    if (item == NULL)
        return QString();

    return item->text();
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
    return this->readTableSelectedId(tbl, 0);
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

void PsmService::searchDepartment(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM departments "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成科室信息检索。" + exterrstr);
    return;
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

bool PsmService::readSelectedPhysioItem(QTableWidget *tbl, PsmSrvPhysioItem *physio)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemprice = tbl->item(rowidx, 2);
    if (itemid == NULL || itemname == NULL)
        return false;

    physio->id = itemid->text();
    physio->name = itemname->text();
    physio->price = 1.0;
    if (itemprice != NULL)
        physio->price = itemprice->text().toDouble();
    return true;
}

QString PsmService::readSelectedPhysioItemId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
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

void PsmService::searchPhysioItem(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM physio_items "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成理疗项目检索。" + exterrstr);
    return;
}

void PsmService::insertNewPhysioItem(const PsmSrvPhysioItem &physio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO physio_items VALUES(?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, physio.id);
    query.bindValue(1, physio.name);
    query.bindValue(2, physio.price);
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
    QMessageBox::warning(window, "数据库错误", "无法添加新的理疗项目。" + exterrstr);
    return;
}

void PsmService::updatePhysioItem(const PsmSrvPhysioItem &physio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE physio_items SET name=?, price=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physio.name);
    query.bindValue(1, physio.price);
    query.bindValue(2, physio.id);
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
    QMessageBox::warning(window, "数据库错误", "无法完成理疗项目信息更新。" + exterrstr);
    return;
}

void PsmService::deletePhysioItem(const QString &physioid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM physio_items WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physioid);
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
    QMessageBox::warning(window, "数据库错误", "无法删除选定理疗项目。" + exterrstr);
    return;
}

bool PsmService::readSelectedDoctor(QTableWidget *tbl, PsmSrvDoctor *doctor)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemtype = tbl->item(rowidx, 2);
    QTableWidgetItem *itemdep = tbl->item(rowidx, 3);
    QTableWidgetItem *itemphone = tbl->item(rowidx, 4);
    if (itemid == NULL || itemname == NULL)
        return false;

    doctor->id = itemid->text();
    doctor->name = itemname->text();
    doctor->isNurse = itemtype->data(Qt::UserRole).toBool();
    doctor->type = itemtype->text();
    doctor->depId = itemdep->data(Qt::UserRole).toString();
    doctor->depName = itemdep->text();
    doctor->phone = itemphone->text();
    return true;
}

QString PsmService::readSelectedDoctorId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshDoctorList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << 4 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM doctors;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成医护人员列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
}

void PsmService::searchDoctor(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << 4 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM doctors "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr OR "
                             "depname LIKE :srchstr OR phonenum LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成医护人员检索。" + exterrstr);
    return;
}

void PsmService::insertDoctor(const PsmSrvDoctor &doctor, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO doctors VALUES(?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, doctor.id);
    query.bindValue(1, doctor.name);
    query.bindValue(2, doctor.isNurse);
    query.bindValue(3, doctor.type);
    query.bindValue(4, doctor.depId);
    query.bindValue(5, doctor.depName);
    query.bindValue(6, doctor.phone);
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
    QMessageBox::warning(window, "数据库错误", "无法添加新的医护人员。" + exterrstr);
    return;
}
