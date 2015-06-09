
/*
Copyright (c) 2011, Volker Götz
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "spellcheckdialog.h"
#include "ui_spellcheckdialog.h"

#include "C_SpellChecker.h"
SpellCheckDialog::SpellCheckDialog(C_SpellChecker *spellChecker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpellCheckDialog)
{
    ui->setupUi(this);
    _spellChecker = spellChecker;

    connect(ui->listWidget, SIGNAL(currentTextChanged(QString)), ui->ledtReplaceWith, SLOT(setText(QString)));

    connect(ui->btnAddToDict, SIGNAL(clicked()), this, SLOT(addToDict()));
    connect(ui->btnReplaceOnce, SIGNAL(clicked()), this, SLOT(replaceOnce()));
    connect(ui->btnReplaceAll, SIGNAL(clicked()), this, SLOT(replaceAll()));
    connect(ui->btnIgnoreOnce, SIGNAL(clicked()), this, SLOT(ignoreOnce()));
    connect(ui->btnIgnoreAll, SIGNAL(clicked()), this, SLOT(ignoreAll()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}


SpellCheckDialog::~SpellCheckDialog()
{
    delete ui;
}


SpellCheckDialog::SpellCheckAction SpellCheckDialog::checkWord(const QString &word)
{
    _unkownWord = word;
    ui->lblUnknownWord->setText(QString("<b>%1</b>").arg(_unkownWord));

    ui->ledtReplaceWith->clear();

    QStringList suggestions = _spellChecker->suggest(word);
    ui->listWidget->clear();
    ui->listWidget->addItems(suggestions);

    if(suggestions.count() > 0)
        ui->listWidget->setCurrentRow(0, QItemSelectionModel::Select);

    _returnCode = AbortCheck;
    QDialog::exec();
    return _returnCode;
}


QString SpellCheckDialog::replacement() const
{
    return ui->ledtReplaceWith->text();
}


void SpellCheckDialog::ignoreOnce()
{
    _returnCode = IgnoreOnce;
    accept();
}


void SpellCheckDialog::ignoreAll()
{
    _spellChecker->ignoreWord(_unkownWord);
    _returnCode = IgnoreAll;
    accept();
}


void SpellCheckDialog::replaceOnce()
{
    _returnCode = ReplaceOnce;
    accept();
}


void SpellCheckDialog::replaceAll()
{
    _returnCode = ReplaceAll;
    accept();
}


void SpellCheckDialog::addToDict()
{
    _spellChecker->addToUserWordlist(_unkownWord);
    _returnCode = AddToDict;
    accept();
}
