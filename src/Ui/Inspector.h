#ifndef __INSPECTOR__
#define __INSPECTOR__

#include <QToolBox>

class Inspector :public QToolBox {
	Q_OBJECT

public:
	Inspector(QWidget* parent = 0);
};


#endif // !__INSPECTOR__
