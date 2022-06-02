#ifndef MAXIMEXPORTER_H
#define MAXIMEXPORTER_H

#include "../abstractexporter.h"

class MaximExporter : public AbstractExporter
{
Q_OBJECT
public:
    explicit MaximExporter(QObject *parent = 0);

    virtual bool Export(QByteArray& out);

private:
    char getSymboldAppendId(QTextCodec* codec, uint id);

signals:

public slots:

};

#endif // MAXIMEXPORTER_H
