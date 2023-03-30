#ifndef  __HELP__DIALOG__
#define __HELP__DIALOG__

#include <QDialog>
#include "UiHelpDialog.h"
#include <QFile>
#include <QIcon>
#include <QDesktopServices>

class HelpDialog :public QDialog {
	Q_OBJECT

public:
	HelpDialog(QDialog* parent=nullptr);
	~HelpDialog();

private:
	Ui::HelpDialog* ui;

	void openLink(const QUrl& url);
};

#endif // ! __HELP__DIALOG__
