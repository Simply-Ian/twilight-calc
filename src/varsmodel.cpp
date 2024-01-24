#include "varsmodel.h"

varsModel::varsModel(const mathModel* m) : mathDataModel(m){}

int varsModel::rowCount(const QModelIndex &parent=QModelIndex()) const {
    return math->get_vars_map().size();
}

QVariant varsModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();
    if (role == Qt::DisplayRole) {
        auto iter = math->get_vars_map().begin();
        std::advance(iter, index.row());
        return QString::fromStdString(iter->first + ": ") + QString::number(iter->second, 'f');
    }
    return QVariant();
}

void varsModel::newRow() {
    beginInsertRows (QModelIndex(), rowCount(), rowCount());
    newVarInserted = true;
}

void varsModel::fixNewRow() {
    if (newVarInserted)
        endInsertRows ();
    newVarInserted = false;
}
