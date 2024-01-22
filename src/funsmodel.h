#ifndef FUNSVARSMODEL_H
#define FUNSVARSMODEL_H
#include "mathdatamodel.h"

class funsModel : public mathDataModel
{
    Q_OBJECT
public:
    funsModel(const mathModel* m);
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
signals:

};

#endif // FUNSVARSMODEL_H
