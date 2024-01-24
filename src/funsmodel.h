#ifndef FUNSVARSMODEL_H
#define FUNSVARSMODEL_H
#include "mathdatamodel.h"

/// @brief Обертка над mathModel, предоставляющая доступ к словарю доступных математических функций в стиле QAbstractListModel
class funsModel : public mathDataModel
{
    Q_OBJECT
public:
    funsModel(const mathModel* m);
    /// @note Не показывает математические операторы (вроде "+", "-", ...), хотя они хранятся вместе с остальными математическими функциями
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
signals:

};

#endif // FUNSVARSMODEL_H
