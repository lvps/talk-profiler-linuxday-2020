#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets>

class Via
{
public:
    Via(QString name, QString comune, QString provincia, QString regione) : name(name), comune(comune), provincia(provincia), regione(regione) {}
    QString getName() const { return name; }
    QString getComune() const { return comune; }
    QString getProvincia() const { return provincia; }
    QString getRegione() const { return regione; }

private:
    QString name;
    QString comune;
    QString provincia;
    QString regione;
};

class TheModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TheModel(QWidget *parent) : QAbstractTableModel(parent) {}
    void setVie(QList<Via> list, QList<QString> duplicates);

    int rowCount(const QModelIndex &parent = QModelIndex()) const { if(parent.isValid()) { return 0; } return vie.size(); }
    int columnCount(const QModelIndex &parent) const { Q_UNUSED(parent); return 4; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QList<Via> vie;
    QList<QString> duplicates;
};

class TheProxy : public QSortFilterProxyModel
{
public:
    TheProxy(QWidget *parent) : QSortFilterProxyModel(parent) {}
};

#endif // TABLE_H
