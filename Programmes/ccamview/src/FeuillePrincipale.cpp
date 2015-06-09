#include "FeuillePrincipale.h"

#include <qvariant.h>
#include <../../MedinTuxTools/CMaskedLineEdit.h>
/*
 *  Constructs a FeuillePrincipale as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
FeuillePrincipale::FeuillePrincipale(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
}

/*
 *  Destroys the object and frees any allocated resources
 */
FeuillePrincipale::~FeuillePrincipale()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FeuillePrincipale::languageChange()
{
    retranslateUi(this);
}

