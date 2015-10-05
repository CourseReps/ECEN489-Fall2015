#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>


class DbManager
{
public:

    DbManager(const QString& path);


    ~DbManager();

    bool isOpen() const;

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H


