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

#include "outputframe.h"
#include "ui_outputframe.h"

#include "outputconfig.h"

#include <QFileDialog>
#include <QImage>
#include <QImageWriter>

#include <iostream>
OutputFrame::OutputFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OutputFrame)
{
    m_config = 0;
    ui->setupUi(this);
    /*foreach( QByteArray format , QImageWriter::supportedImageFormats()) {
        QString name = format;
        ui->comboBoxImageFormat->addItem(name,format);
    }*/
}

OutputFrame::~OutputFrame()
{
    delete ui;
}

void OutputFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void OutputFrame::setConfig(OutputConfig* config) {
    config->setWriteImage(true);
    config->setImageFormat("BMP");
    config->setWriteDescription(true);
    config->setDescriptionFormat("Maxim Exporter");

    m_config = config;
    if (config) {
        ui->lineEditPath->setText(config->path());
        connect(config,SIGNAL(imageNameChanged(QString)),this,SLOT(onImageNameChanged(QString)));
        onImageNameChanged(config->imageName());
        connect(config,SIGNAL(descriptionNameChanged(QString)),this,SLOT(onDescriptionNameChanged(QString)));

        ui->checkBoxGenerateX2->setChecked(config->generateX2());

        ui->widgetBackgroundColor->setColor(config->bgColor());
        ui->widgetForegroundColor->setColor(config->fgColor());
        connect(ui->widgetBackgroundColor,SIGNAL(colorChanged(QColor)),this,SLOT(setBackgroundColor(QColor)));
        connect(ui->widgetForegroundColor,SIGNAL(colorChanged(QColor)),this,SLOT(setForegroundColor(QColor)));

    }
}

void OutputFrame::on_pushButtonSelectPath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
            this,tr("Select output directory"),
            ui->lineEditPath->text());
    if (!dir.isEmpty()) {
        if (m_config) m_config->setPath(dir);
        ui->lineEditPath->setText(dir);
    }
}

void OutputFrame::onImageNameChanged(const QString& s) {
    ui->lineEditImageFilename->setText(s);
}

void OutputFrame::on_lineEditImageFilename_editingFinished()
{
    if (m_config) {
        m_config->setImageName(ui->lineEditImageFilename->text());
        m_config->setDescriptionName(ui->lineEditImageFilename->text());
    }
}

void OutputFrame::on_checkBoxGenerateX2_stateChanged(int arg1)
{
    if (m_config) m_config->setGenerateX2(arg1==Qt::Checked);
}

void OutputFrame::setBackgroundColor(QColor c) {
    std::cout << "background : " << c.rgba() << std::endl;
    if (m_config) m_config->setBgColor(c);
}

void OutputFrame::setForegroundColor(QColor c) {
    std::cout << "foreground : " << c.rgba() << std::endl;
    if (m_config) m_config->setFgColor(c);
}


