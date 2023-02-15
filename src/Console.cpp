#include "Console.h"

Console* Console::console = nullptr;
QString Console::warning ="<div><img src=\":icons/warning.ico\" width=\"15\" height=\"15\">\
		<font size=\"5\">%1%2</font></div>";
QString Console::error ="<div><img src=\":icons/error.ico\" width=\"20\" height=\"20\">\
		<font size=\"5\">%1%2</font></div>";
QString Console::info ="<div><img src=\":icons/info.ico\" width=\"15\" height=\"15\">\
		<font size=\"5\">%1%2</font></div>";


Console::Console(QWidget* parent):QTextBrowser(parent)
{
}

Console::~Console()
{
	Console::destroy();
}

void Console::Warning(const QString& text)
{
	if (!console) {
		return;
	}
	QDateTime datetime = QDateTime::currentDateTime();
	QString now = datetime.toString("[yyyy-MM-dd HH:mm:ss]");
	console->append(warning.arg(now).arg(text));
	QApplication::processEvents(); //显示数据，避免耗时任务太久
}

void Console::Info(const QString& text)
{
	if (!console) {
		return;
	}
	QDateTime datetime = QDateTime::currentDateTime();
	QString now = datetime.toString("[yyyy-MM-dd HH:mm:ss]");
	console->append(info.arg(now).arg(text));
	QApplication::processEvents(); //显示数据，避免耗时任务太久
}

void Console::Error(const QString& text)
{
	if (!console) {
		return;
	}
	QDateTime datetime = QDateTime::currentDateTime();
	QString now = datetime.toString("[yyyy-MM-dd HH:mm:ss]");
	console->append(error.arg(now).arg(text));
	QApplication::processEvents(); //显示数据，避免耗时任务太久
}

void Console::setConsole(Console* newconsole)
{
	if (!console) {
		console = newconsole;
	}
}

void Console::clear()
{
	if (!console) return;
	console->setText("");
}

void Console::destroy()
{
	console = nullptr;
}
