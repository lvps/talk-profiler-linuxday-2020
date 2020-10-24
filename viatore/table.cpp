#include "table.h"

#include <QtWidgets>


void TheModel::setVie(QList<Via> list, QList<QString> duplicates)
{
    beginResetModel();
    this->vie = list;
    this->duplicates = duplicates;
    endResetModel();
}

QVariant TheModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::DisplayRole) {
            if(index.row() < vie.size() && index.column() < 5) {
                switch(index.column()) {
                case 0:
                    return vie.at(index.row()).getName();
                case 1:
                    return vie.at(index.row()).getComune();
                case 2:
                    return vie.at(index.row()).getProvincia();
                case 3:
                    return vie.at(index.row()).getRegione();
                }
            }
        } else if(role == Qt::BackgroundRole) {
            if(duplicates.contains(vie.at(index.row()).getName())) {
                return QColor(255, 170, 0);
            }
        }
    }
    return QVariant();
}

QVariant TheModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal) {
        if(role == Qt::DisplayRole) {
            switch(section) {
            case 0:
                return "Via";
            case 1:
                return "Comune";
            case 2:
                return "Provincia";
            case 3:
                return "Regione";
            }
        }
    }
    return QVariant();
}
