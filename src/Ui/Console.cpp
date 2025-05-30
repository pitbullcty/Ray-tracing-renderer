﻿#include "Console.h"


QString Console::warning = "<div><img src=\":icons/warning.ico\" width=\"15\" height=\"15\">\
		<font size=\"5\">%1%2</font></div>";
QString Console::error = "<div><img src=\":icons/error.ico\" width=\"20\" height=\"20\">\
		<font size=\"5\">%1%2</font></div>";
QString Console::info = "<div><img src=\":icons/info.ico\" width=\"15\" height=\"15\">\
		<font size=\"5\">%1%2</font></div>";

bool isBusy = false; //静态外部变量，程序是否忙碌


Console::Console(QWidget* parent):QTextBrowser(parent)
{
	setOpenExternalLinks(true);
	document()->setMaximumBlockCount(100); //最多100条记录
}

void Console::Warning(const QString& text)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString now = datetime.toString("[yyyy-MM-dd HH:mm:ss]");
	append(warning.arg(now).arg(text));
	isBusy = true;
	QApplication::processEvents(); //显示数据，避免耗时任务太久
	scrollToBottom();
	isBusy = false;
}

void Console::Info(const QString& text, bool needProcess)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString now = datetime.toString("[yyyy-MM-dd HH:mm:ss]");
	append(info.arg(now).arg(text));
	if (text.contains("BVH") || text.contains("保存至") || text.contains("渲染")) isBusy = false; //处理特殊时间
	else isBusy = true;
	if(needProcess) QApplication::processEvents(); //显示数据，避免耗时任务太久
	scrollToBottom();
	isBusy = false;
}

void Console::Error(const QString& text)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString now = datetime.toString("[yyyy-MM-dd HH:mm:ss]");
	append(error.arg(now).arg(text));
	isBusy = true;
	QApplication::processEvents(); //显示数据，避免耗时任务太久
	scrollToBottom();
	isBusy = false;
}

void Console::Clear()
{
	setText("");
}

void Console::openImage(const QUrl& url)
{
	QString text = this->toHtml();
	QDesktopServices::openUrl(url);
	this->setHtml(text);
	scrollToBottom();
}

void Console::scrollToBottom()
{
	moveCursor(QTextCursor::End);
}
