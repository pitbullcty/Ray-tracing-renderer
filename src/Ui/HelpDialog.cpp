#include "HelpDialog.h"

HelpDialog::HelpDialog(QDialog* parent):
	QDialog(parent),
	ui(new Ui::HelpDialog)
{
	ui->setupUi(this);
	setWindowTitle("帮助");
	setWindowIcon(QIcon(":/icons/title.ico"));
	QFile markdown(":/doc/help.md");
	markdown.open(QIODevice::ReadOnly);
	auto text = QString(markdown.readAll());

	ui->textBrowser->setMarkdown(text);
	ui->textBrowser->setOpenExternalLinks(true);

	connect(ui->textBrowser, &QTextBrowser::anchorClicked, this, &HelpDialog::openLink);
}

HelpDialog::~HelpDialog()
{
	delete ui;
}

void HelpDialog::openLink(const QUrl& url)
{
	QString text = ui->textBrowser->toHtml();
	QDesktopServices::openUrl(url);
	ui->textBrowser->setHtml(text);
}
