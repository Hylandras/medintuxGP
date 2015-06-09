#include "Dlg_SigemsLogin.h"

#include <qvariant.h>
/*
 *  Constructs a Dlg_SigemsLogin as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
Dlg_SigemsLogin::Dlg_SigemsLogin(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
Dlg_SigemsLogin::~Dlg_SigemsLogin()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Dlg_SigemsLogin::languageChange()
{
    retranslateUi(this);
}

