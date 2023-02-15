#ifndef __CONSOLE__
#define __CONSOLE__

#include <QTextBrowser>
#include <QApplication>

class Console :public QTextBrowser {
	Q_OBJECT

public:
	static Console* console;
	static QString info;
	static QString warning;
	static QString error;

	Console(QWidget* parent = 0);
	~Console();
	static void Warning(const QString& text);
	static void Info(const QString& text);
	static void Error(const QString& text);
	static void setConsole(Console* newconsole);
	static void clear();

private:
	static void destroy();
};


#endif 