/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "dialogfontselect.h"
#include "ui_dialogfontselect.h"

#include "charactersmodel.h"
//-----------------------------------------------------------------------------
DialogFontSelect::DialogFontSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontSelect)
{
    ui->setupUi(this);

    this->mModel = new CharactersModel(this);
    this->ui->tableView->setModel(this->mModel);

    this->ui->radioButtonMonospaced->setChecked(false);
    this->ui->radioButtonProportional->setChecked(false);
    this->ui->checkBoxAntialiasing->setChecked(false);

    this->mSize = 14;
    this->mMonospaced = false;
    this->mAntialiasing = false;

    QString defChars;
    for (int i = 0x20; i < 0x7f; i++)
    {
        defChars += QString(QChar(i));
    }
    this->ui->lineEdit->setText(defChars);

    this->mFontFamily = this->ui->fontComboBox->currentFont().family();
    this->updateStyles();

    int cellSize = qMax(24, QFontMetrics(this->ui->tableView->font()).height());
    for (int i = 0; i < 32; i++)
        this->ui->tableView->setColumnWidth(i , cellSize);
    //this->ui->tableView->resizeColumnsToContents();
}
//-----------------------------------------------------------------------------
DialogFontSelect::~DialogFontSelect()
{
    delete ui;
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::fontFamily()
{
    return this->mFontFamily;
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::fontStyle()
{
    return this->mFontStyle;
}
//-----------------------------------------------------------------------------
int DialogFontSelect::fontSize()
{
    return this->mSize;
}
//-----------------------------------------------------------------------------
bool DialogFontSelect::monospaced()
{
    return this->mMonospaced;
}
//-----------------------------------------------------------------------------
bool DialogFontSelect::antialiasing()
{
    return this->mAntialiasing;
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::characters()
{
    return this->mCharacters;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setFontFamily(const QString &value)
{
    QFontDatabase fonts;
    QString style = this->mFontStyle;
    QFont f = fonts.font(value, style, this->mSize);
    //this->mFontFamily = value;
    this->ui->fontComboBox->setCurrentFont(f);

    this->updateStyles();
    this->updateSizes();

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setFontStyle(const QString &value)
{
    this->mFontStyle = value;
    int index = this->ui->comboBoxStyle->findText(value);
    if (index >= 0)
        this->ui->comboBoxStyle->setCurrentIndex(index);
    else
        this->ui->comboBoxStyle->setCurrentIndex(0);

    this->updateSizes();

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setFontSize(int value)
{
    this->mSize = value;

    int index = this->ui->comboBoxSize->findText(QString("%1").arg(value));
    if (index >= 0)
        this->ui->comboBoxSize->setCurrentIndex(index);
    else
        this->ui->comboBoxSize->setCurrentIndex(0);

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setMonospaced(bool value)
{
    //this->mMonospaced = value;
    this->ui->radioButtonMonospaced->setChecked(value);
    //this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setAntialising(bool value)
{
    //this->mAntialiasing = value;
    this->ui->checkBoxAntialiasing->setChecked(value);
    //this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setCharacters(const QString &value)
{
    this->mCharacters = value;
    this->ui->lineEdit->setText(value);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateFont()
{
    //QFontDatabase fonts;
    //QFont font = fonts.font(this->mFontFamily, this->mFontStyle, this->mSize);
    //this->ui->fontComboBox->setCurrentFont(font);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateStyles()
{
    QString style = this->mFontStyle;
    this->ui->comboBoxStyle->clear();

    QFontDatabase fonts;
    QStringList stylesList = fonts.styles(this->mFontFamily);
    this->ui->comboBoxStyle->addItems(stylesList);
    int index = stylesList.indexOf(style);
    if (index >= 0)
        this->ui->comboBoxStyle->setCurrentIndex(stylesList.indexOf(style));
    else
        this->ui->comboBoxStyle->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateSizes()
{
    int a = this->mSize;
    //bool ok;

    QFontDatabase fonts;

    QList<int> sizes;
    if (fonts.isSmoothlyScalable(this->mFontFamily, this->mFontStyle))
        sizes = fonts.smoothSizes(this->mFontFamily, this->mFontStyle);
    else
        sizes = fonts.standardSizes();
    this->ui->comboBoxSize->clear();
    QListIterator<int> it(sizes);
    it.toFront();
    int b = 0, c = 0;
    while (it.hasNext())
    {
        int size = it.next();
        this->ui->comboBoxSize->addItem(QString("%1").arg(size), QVariant(size));
        if (a == 14 && c == 0)
            c = b;
        if (size == a)
            c = b;
        b++;
    }
    this->ui->comboBoxSize->setCurrentIndex(c);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::applyFont()
{
    QFontDatabase fonts;
    QFont font = fonts.font(this->mFontFamily, this->mFontStyle, this->mSize);
    this->ui->tableView->setFont(font);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_fontComboBox_currentFontChanged(const QFont &font)
{
    this->mFontFamily = font.family();

    this->updateStyles();
    this->updateSizes();

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_comboBoxSize_currentIndexChanged(int index)
{
    bool ok;
    int a = this->ui->comboBoxSize->itemData(index).toInt(&ok);
    if (ok)
        this->mSize = a;

    this->applyFont();

    int cellSize = qMax(24, QFontMetrics(this->ui->tableView->font()).height());
    for (int i = 0; i < 32; i++)
        this->ui->tableView->setColumnWidth(i , cellSize);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_comboBoxStyle_currentIndexChanged(const QString &text)
{
    this->mFontStyle = text;

    this->updateSizes();

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_radioButtonProportional_toggled(bool value)
{
    this->mMonospaced = !value;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_checkBoxAntialiasing_toggled(bool value)
{
    this->mAntialiasing = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_lineEdit_textChanged(const QString &value)
{
    this->mCharacters = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString str = this->ui->lineEdit->text();
    QString a = this->mModel->data(index, Qt::DisplayRole).toString();
    this->ui->lineEdit->setText(str + a);
}
//-----------------------------------------------------------------------------