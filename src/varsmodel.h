#ifndef VARSMODEL_H
#define VARSMODEL_H
#include "mathdatamodel.h"


class varsModel : public mathDataModel
{
    Q_OBJECT
public:
    varsModel(const mathModel* m);
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
};

#endif // VARSMODEL_H
