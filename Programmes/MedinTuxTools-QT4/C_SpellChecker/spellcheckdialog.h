
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

#ifndef SPELLCHECKDIALOG_H
#define SPELLCHECKDIALOG_H

#include <QDialog>

class C_SpellChecker;

namespace Ui {
    class SpellCheckDialog;
}
//============================== SpellCheckDialog ============================================
class SpellCheckDialog : public QDialog
{
    Q_OBJECT

public:
    enum     SpellCheckAction {AbortCheck, IgnoreOnce, IgnoreAll, ReplaceOnce, ReplaceAll, AddToDict};

    explicit SpellCheckDialog(C_SpellChecker *spellChecker, QWidget *parent = 0);
    ~SpellCheckDialog();

    QString replacement() const;

public slots:
    SpellCheckAction checkWord(const QString &word);

protected slots:
    void ignoreOnce();
    void ignoreAll();
    void replaceOnce();
    void replaceAll();
    void addToDict();

private:
    Ui::SpellCheckDialog *ui;
    C_SpellChecker       *_spellChecker;
    QString               _unkownWord;
    SpellCheckAction      _returnCode;
};

#endif // SPELLCHECKDIALOG_H
