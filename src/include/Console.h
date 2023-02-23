#ifndef __CONSOLE__
#define __CONSOLE__

#include <QTextBrowser>
#include <QApplication>
#include <QDateTime>
#include <QTimer>


class Console :public QTextBrowser {
	Q_OBJECT

public:
	static Console* console;
	static QString info;
	static QString warning;
	static QString error;

	Console(QWidget* parent = 0);
	static void Warning(const QString& text);
	static void Info(const QString& text);
	static void Error(const QString& text);
	static void setConsole(Console* newconsole);
	static void clear();
	

private:
	static void destroy();
	static void scroll();
};


#endif 