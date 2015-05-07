#ifndef GPROPERTIESOBJ_H_
#define GPROPERTIESOBJ_H_

#include <QLayout>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>

#include "GVectorFigure.h"
#include "GContainer.h"

class PropertiesObjDialog:public QDialog
{
	Q_OBJECT

	public:

		static int changePropertiesObj( GContainer *currentObject, const QString title, QWidget *parent = 0 )
		{
			if( currentObject == 0 )
				return -1;
			int _alpha = currentObject->getAlpha();
			PropertiesObjDialog dialog( currentObject, parent, _alpha);
			dialog.setWindowTitle( title );
			if( dialog.exec() == Rejected )
				return _alpha;
			else
				return currentObject->getAlpha();
		}


	private:

		QSpinBox countAlpha;
		QLabel label;
		QDialogButtonBox buttons;
		GContainer *_obj;




        PropertiesObjDialog( GContainer *currentObject, QWidget *parent = 0 ,int _alpha = 0):QDialog( parent ),
						countAlpha(this),label(this),
						buttons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
							Qt::Horizontal, this )
				{

					_obj = currentObject;
					QVBoxLayout *v = new QVBoxLayout( this );
					QHBoxLayout *l = new QHBoxLayout( this );

					label.setText(tr("Alpha count:"));
					l->addWidget( &label );

					countAlpha.setMaximum(255);
					countAlpha.setMinimum(0);
					countAlpha.setValue(_alpha);
					l->addWidget( &countAlpha );

					v->addLayout(l);

					l = new QHBoxLayout( this );
					l->addWidget( &buttons );
					v->addLayout(l);

					connect( &buttons, SIGNAL( accepted() ), this, SLOT( accept() ) );
					connect( &buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );
					connect( &countAlpha, SIGNAL( valueChanged ( int ) ),
							parent, SLOT( alphaChanged ( int ) ) );

				}

		virtual ~PropertiesObjDialog()
		{
		}
};


#endif /* GPROPERTIESOBJ_H_ */
