#include "funsmodel.h"
#include <iterator>

funsModel::funsModel(const mathModel* m) : mathDataModel(m)
{

}

int funsModel::rowCount(const QModelIndex &parent=QModelIndex()) const {
    return math->get_mathfuns_map().size() - 14;
}

QVariant funsModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();
    if (role == Qt::DisplayRole) {
        auto iter = math->get_mathfuns_map().begin();
        std::advance(iter, index.row() + 14);
        QString base_signature = QString::fromStdString(iter->first + "(");
        for (int i = 0; i < iter->second.get_args_num() - 1; i++) {
            base_signature += QChar(0x61 + i);
            base_signature += ", ";
        }
        base_signature += QChar(0x61 + iter->second.get_args_num());
        return base_signature + ')';
    }
    if (role == Qt::ToolTipRole) {
        auto iter = math->get_mathfuns_map().begin();
        std::advance(iter, index.row() + 14);
        return QString::fromStdString(iter->second.description);
    }
    return QVariant();
}
