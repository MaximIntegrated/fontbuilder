/**
 * Copyright (c) 2010-2010 Andrey AndryBlack Kunitsyn
 * email:support.andryblack@gmail.com
 *
 * Report bugs and download new versions at http://code.google.com/p/fontbuilder
 *
 * This software is distributed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "maximexporter.h"
#include "../fontconfig.h"
#include <QtXml>
#include <iostream>

MaximExporter::MaximExporter(QObject *parent) :
    AbstractExporter(parent)
{
    setExtension("mff");
}

char MaximExporter::getSymboldAppendId(QTextCodec* codec, uint id) {
    uint symbol_id[] = {id};
    QString symbol_str = QString::fromUcs4(symbol_id, 1);
    QByteArray result = codec->fromUnicode(symbol_str);

    // !!!!!!!!!!!!!!!!!
    // Assumption :: Since only 8 bit code-page is supported first byte of result will be taken!
    char symbol_append_id = result.size() > 1 ? 0 : result[0];
    return symbol_append_id;
}

bool MaximExporter::Export(QByteArray& out) {
    QString codepage = fontConfig()->codepage();
    QTextCodec* codec = QTextCodec::codecForName(codepage.toUtf8());

    const QVector<Symbol>& characters = symbols();

    out.append(1, 0x06);
    int size = characters.length() - 1; // Space character is not supported on embedded platform
    out.append((char*)&size, 1);
    out.append(4, 0x00);
    foreach(const Symbol& c, characters) {
        if(c.id == 32) { continue; } // Space character is not supported on embedded platform

        out.append((char*)&c.placeX, 2);
        out.append((char*)&c.placeW, 1);

        char symbol_append_id = getSymboldAppendId(codec, c.id);
        out.append(1, symbol_append_id);
    }
    return true;
}


AbstractExporter* MaximExporterFactoryFunc (QObject* parent) {
    return new MaximExporter(parent);
}
