#ifndef __CONSOLE__
#define __CONSOLE__

#include <QTextBrowser>
#include <QApplication>
#include <QDateTime>
#include <QTimer>


class Console :public QTextBrowser {
	Q_OBJECT

public:
	static QString info;
	static QString warning;
	static QString error;

	Console(QWidget* parent = 0);


public slots:
	void Warning(const QString& text);
	void Info(const QString& text);
	void Error(const QString& text);
	void Clear();
	

private:
	void scroll();
};


#endif 