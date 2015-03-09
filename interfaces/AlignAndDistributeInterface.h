#ifndef __ALIGNANDDISTRIBUTE_INTERFACE_H__
#define __ALIGNANDDISTRIBUTE_INTERFACE_H__

/**
*	@file AlignAndDistribute.h Р’ РґР°РЅРЅРѕРј С„Р°Р№Р»Рµ РѕРїРёСЃР°РЅ РёРЅС‚РµСЂС„РµР№СЃ, РєРѕС‚РѕСЂС‹Р№ РґРѕР»Р¶РµРЅ СЂРµР°Р»РёР·РѕРІС‹РІР°С‚СЊ
*		Р»СЋР±РѕР№ РїР»Р°РіРёРЅ, РѕС‚РІРµС‡Р°СЋС‰РёР№ Р·Р° Р’С‹СЂРѕРІРЅСЏС‚СЊ Рё СЂР°СЃСЃС‚Р°РІРёС‚СЊ.
*/

/**
*		РРЅС‚РµСЂС„РµР№СЃ, РєРѕС‚РѕСЂС‹Р№ РґРѕР»Р¶РµРЅ СЂРµР°Р»РёР·РѕРІС‹РІР°С‚СЊ Р»СЋР±РѕР№ РїР»Р°РіРёРЅ, РѕС‚РІРµС‡Р°СЋС‰РёР№ Р·Р° Р’С‹СЂРѕРІРЅСЏС‚СЊ Рё СЂР°СЃСЃС‚Р°РІРёС‚СЊ.
*	РџР»Р°РіРёРЅ, СЂРµР°Р»РёР·СѓСЋС‰РёР№ РґР°РЅРЅС‹Р№ РёРЅС‚РµСЂС„РµР№СЃ РґРѕР»Р¶РµРЅ РЅР°СЃР»РµРґРѕРІР°С‚СЊСЃСЏ РѕС‚ Р»СЋР±РѕРіРѕ РїРѕС‚РѕРјРєР° РєР»Р°СЃСЃР°
*	QWidget РёР»Рё РѕС‚ РЅРµРіРѕ СЃР°РјРѕРіРѕ.
*
*		РљСЂРѕРјРµ СЂРµР°Р»РёР·Р°С†РёРё РґР°РЅРЅРѕРіРѕ РёРЅС‚РµСЂС„РµР№СЃР°, Р»СЋР±РѕР№ С‚Р°РєРѕР№ РїР»Р°РіРёРЅ РґРѕР»Р¶РµРЅ РїСЂРµРґРѕСЃС‚Р°РІР»СЏС‚СЊ
*	СЃР»РµРґСѓСЋС‰РёРµ СЃРёРіРЅР°Р»С‹:
*
*	@code
*
*
*	@endcode
*/

class AlignAndDistributeInterface
{
	public:
		/**
		*	РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ.
		*/
		AlignAndDistributeInterface() {}
		
		//virtual int getFps() = 0;
		/**
		*	Р”РµСЃС‚СЂСѓРєС‚РѕСЂ.
		*/
		virtual ~AlignAndDistributeInterface(){}
};

Q_DECLARE_INTERFACE( AlignAndDistributeInterface, "com.alxn1.MultiFusion.AlignAndDistributeInterface/1.0" )

/**
*	РџСЂРёРІРѕРґРёС‚ СѓРєР°Р·Р°С‚РµР»СЊ РЅР° Р»СЋР±РѕР№ РѕР±СЊРµРєС‚ Рє СѓРєР°Р·Р°С‚РµР»СЊСЋ РЅР° РёРЅС‚РµСЂС„РµР№СЃ TimelineInterface.
*
*	@return РџСЂРёРІРµРґС‘РЅРЅС‹Р№ СѓРєР°Р·Р°С‚РµР»СЊ РёР»Рё 0, РµСЃР»Рё РїСЂРѕРёР·РѕС€Р»Р° РѕС€РёР±РєР°.
*/
template < class T > inline AlignAndDistributeInterface* TIMELINE( T *o )
{
	return qobject_cast< AlignAndDistributeInterface* >( o );
}

#endif /* __ALIGNANDDISTRIBUTE_INTERFACE_H__ */
