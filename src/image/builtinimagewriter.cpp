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


#include "builtinimagewriter.h"
#include "layoutdata.h"
#include <QPainter>
#include "../layoutconfig.h"
#include <iostream>

BuiltinImageWriter::BuiltinImageWriter(QString format,QString ext,QObject *parent, const OutputConfig* config) :
    AbstractImageWriter(parent, config)
{
    setExtension(ext);
    setReloadSupport(true);
    m_format = format;
}


bool BuiltinImageWriter::Export(QFile& file) {

    QImage pixmap = buildImage(m_config->bgColor().rgba());

    // TODO : use both fg and bg color
    if(m_format == "bmp" || m_format == "BMP") {

        QVector<QRgb> colorTable;
        int32_t rDiff = m_config->bgColor().red() - m_config->fgColor().red();
        int32_t gDiff = m_config->bgColor().green() - m_config->fgColor().green();
        int32_t bDiff = m_config->bgColor().blue() - m_config->fgColor().blue();

        for(int i = 0; i < 256; i++) {
            colorTable.append(0xff << 24
                              | ((m_config->fgColor().red() + (rDiff*i)/255) & 0xFF) << 16
                              | ((m_config->fgColor().green() + (gDiff*i)/255) & 0xFF) << 8
                              | ((m_config->fgColor().blue() + (bDiff*i)/255) & 0xFF));
            //std::cout << colorTable.takeLast() << std::endl;
        }

        QImage bitmap = pixmap.convertToFormat(QImage::Format_Indexed8, colorTable);
        bitmap.save(&file, "bmp");
        std::cout << "save bmp file" << std::endl;
    }
    else {
        pixmap.save(&file,m_format.toUtf8().data());
        std::cout << "save "<< m_format.toUtf8().data()<< " file" << std::endl;
    }
    return true;
}

QImage* BuiltinImageWriter::reload(QFile& file) {
    QImage* img = new QImage();
    if (img->load(&file,m_format.toUtf8().data()))
        return img;
    delete img;
    return 0;
}
