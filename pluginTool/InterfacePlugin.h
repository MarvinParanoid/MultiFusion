#ifndef INTERFACEPLUGIN_H_
#define INTERFACEPLUGIN_H_

#include "Plugin.h"
#include "SignalHolder.h"

class QObject;

class InterfacePlugin
{
public:
    virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager) = 0;
    virtual QString getName()const = 0;

    /**
     * Returns the SignalHolder for this plugin (see below).
     */
    SignalHolder *getBeep() {
    	return beep;
    }

    virtual ~InterfacePlugin() {
		delete beep;
	};
	InterfacePlugin() {
		beep = new SignalHolder();
	};
protected:
	/**
	 * A special aggregated object used to emit signals from base plugins.
	 * All signals generic for the whole application's plugin set should be registered in this class.
	 */
	SignalHolder *beep;
};

Q_DECLARE_INTERFACE(InterfacePlugin,"com.alxn1.DocWindow.PluginInterface/1.0")


template < class T > inline InterfacePlugin * PLUGIN( T *o )
{
	return qobject_cast< InterfacePlugin* >( o );
}

#endif /* INTERFACEPLUGIN_H_ */
