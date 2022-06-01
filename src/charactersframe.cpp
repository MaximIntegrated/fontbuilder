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

#include "charactersframe.h"
#include "ui_charactersframe.h"
#include <QFileDialog>
#include <QDebug>
#include "fontconfig.h"
#include "charmapdialog.h"
#include "iostream"
#include "QTextCodec"
CharactersFrame::CharactersFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CharactersFrame)
{
    ui->setupUi(this);
    m_config = 0;

    QList<QString> encodings(
    {
        // http://www.unicode.org/Public/UCD/latest/ucd/Blocks.txt (on Feb 2014)
        // Omit NUL
        "UTF-8" ,
        "ISO-8859-1" ,
        "latin1" ,
        "CP819" ,
        "IBM819" ,
        "iso-ir-100" ,
        "csISOLatin1" ,
        "ISO-8859-15" ,
        "latin9" ,
        "System" ,
        "Shift_JIS" ,
        "hp-roman8" ,
        "roman8" ,
        "csHPRoman8" ,
        "TIS-620" ,
        "ISO 8859-11" ,
        "macintosh" ,
        "Apple Roman" ,
        "MacRoman" ,
        "windows-1258" ,
        "CP1258" ,
        "windows-1257" ,
        "CP1257" ,
        "windows-1256" ,
        "CP1256" ,
        "windows-1255" ,
        "CP1255" ,
        "windows-1254" ,
        "CP1254" ,
        "windows-1253" ,
        "CP1253" ,
        "windows-1252" ,
        "CP1252" ,
        "windows-1251" ,
        "CP1251" ,
        "windows-1250" ,
        "CP1250" ,
        "IBM866" ,
        "CP866" ,
        "csIBM866" ,
        "IBM874" ,
        "CP874" ,
        "IBM850" ,
        "CP850" ,
        "csPC850Multilingual" ,
        "ISO-8859-16" ,
        "iso-ir-226" ,
        "latin10" ,
        "ISO-8859-14" ,
        "iso-ir-199" ,
        "latin8" ,
        "iso-celtic" ,
        "ISO-8859-13" ,
        "ISO-8859-10" ,
        "iso-ir-157" ,
        "latin6" ,
        "ISO-8859-10:1992" ,
        "csISOLatin6" ,
        "ISO-8859-9" ,
        "iso-ir-148" ,
        "latin5" ,
        "csISOLatin5" ,
        "ISO-8859-8" ,
        "ISO 8859-8-I" ,
        "iso-ir-138" ,
        "hebrew" ,
        "csISOLatinHebrew" ,
        "ISO-8859-7" ,
        "ECMA-118" ,
        "greek" ,
        "iso-ir-126" ,
        "csISOLatinGreek" ,
        "ISO-8859-6" ,
        "ISO-8859-6-I" ,
        "ECMA-114" ,
        "ASMO-708" ,
        "arabic" ,
        "iso-ir-127" ,
        "csISOLatinArabic" ,
        "ISO-8859-5" ,
        "cyrillic" ,
        "iso-ir-144" ,
        "csISOLatinCyrillic" ,
        "ISO-8859-4" ,
        "latin4" ,
        "iso-ir-110" ,
        "csISOLatin4" ,
        "ISO-8859-3" ,
        "latin3" ,
        "iso-ir-109" ,
        "csISOLatin3" ,
        "ISO-8859-2" ,
        "latin2" ,
        "iso-ir-101" ,
        "csISOLatin2" ,
        "KOI8-U" ,
        "KOI8-RU" ,
        "KOI8-R" ,
        "csKOI8R" ,
        "iscii-mlm" ,
        "iscii-knd" ,
        "iscii-tlg" ,
        "iscii-tml" ,
        "iscii-ori" ,
        "iscii-gjr" ,
        "iscii-pnj" ,
        "iscii-bng" ,
        "iscii-dev" ,
        "TSCII" ,

    });
    QStringList encodingList(encodings);

    ui->comboBox_TextEncoding->addItems(encodings);

}

CharactersFrame::~CharactersFrame()
{
    delete ui;
}

void CharactersFrame::changeEvent(QEvent *e)
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

void CharactersFrame::on_pushButtonImport_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Select file"),
                                                QString(),
                                                tr("Text file(*.txt)"));
    if (!file.isEmpty()) {
        QFile f(file);
        if (f.open(QFile::ReadOnly | QFile::Text)) {
            QByteArray data = f.readAll();
            QString text = QString::fromUtf8(data.constData(),data.size());
            text = removeDuplicates(sortChars(text));
            ui->plainTextEdit->setPlainText(text);
        }
    }
}

void CharactersFrame::on_pushButtonExport_clicked()
{
    QString file = QFileDialog::getSaveFileName(this,tr("Select file"),
                                                QString(),
                                                tr("Text file(*.txt)"));
    if (!file.isEmpty()) {
        QFile f(file);
        if (f.open(QFile::WriteOnly | QFile::Text)) {
            QByteArray data =
                ui->plainTextEdit->document()->toPlainText().toUtf8();
            f.write(data);
        }
    }
}


QString CharactersFrame::getCharacters() const {
    return ui->plainTextEdit->document()->toPlainText();
}

void CharactersFrame::on_plainTextEdit_textChanged()
{
    if (m_config) {
        m_config->setCharacters(removeDuplicates(sortChars(getCharacters())));
    }
}

void CharactersFrame::setConfig(FontConfig* config) {
    m_config = config;
    ui->plainTextEdit->setPlainText(config->characters());
}

void CharactersFrame::setDefault()
{
    ui->plainTextEdit->setPlainText(m_config->defaultCharacters());
}

QString CharactersFrame::removeDuplicates(const QString& text) const {
    std::vector<uint> ucs4chars = text.toUcs4().toStdVector();

    // Remove duplicates with C++ algorithm
    std::vector<uint>::const_iterator newEnd;
    newEnd = std::unique(ucs4chars.begin(), ucs4chars.end());

    // Drop NUL character(s) at the beginning
    std::vector<uint>::const_iterator newStart = ucs4chars.begin();
    while (newStart != newEnd && *newStart == 0)
        ++newStart;

    return QString::fromUcs4(&*newStart, newEnd - newStart);
}

QString CharactersFrame::sortChars(const QString& text) const {
    return text;

    QVector<uint> ucs4chars = text.toUcs4();
    qSort(ucs4chars);
    return QString::fromUcs4(&ucs4chars.front(), ucs4chars.size());
}


void CharactersFrame::on_pushButtonDefault_clicked()
{
    setDefault();
}

void CharactersFrame::on_pushButton_SelectFromCharsMap_clicked()
{
    CharMapDialog dialog(this);
    dialog.setModal(true);
    dialog.setChars(m_config->characters());
    int result = dialog.exec();
    (void)result;
    if (dialog.result()==QDialog::Accepted) {
        m_config->setCharacters(removeDuplicates(sortChars(dialog.getCharacters())));
        bool block = ui->plainTextEdit->blockSignals(true);
        ui->plainTextEdit->setPlainText(m_config->characters());
        ui->plainTextEdit->blockSignals(block);
    }
}

void CharactersFrame::on_pushButtonRefresh_clicked()
{
    bool block = ui->plainTextEdit->blockSignals(true);
    ui->plainTextEdit->setPlainText(m_config->characters());
    ui->plainTextEdit->blockSignals(block);
}

void CharactersFrame::on_comboBox_TextEncoding_currentTextChanged(const QString& text){
    QTextCodec* codec = QTextCodec::codecForName(text.toUtf8());
    QString result("");
    for(int i = 0x20; i <= 0xFF; i++) {
        char item[] = {(char)i};
        QString itemucs4 = codec->toUnicode(item, 1);
        result.append(itemucs4);
    }

    ui->plainTextEdit->setPlainText(result);
}
