// MyDlgPosoAdjust.h

// QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0

DlgPosoAdjust::DlgPosoAdjust(Type_Posologie &type_Posologie);

DlgPosoAdjust::DlgPosoAdjust(Type_Posologie &type_Posologie)
{m_Poso         = type_Posologie;
 DlgPosoAdjust();
}