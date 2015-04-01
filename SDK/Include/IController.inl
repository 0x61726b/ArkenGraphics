//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IController.inl
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
template <typename T>
IController<T>::IController()
{
	m_pEntity = 0;
}
//--------------------------------------------------------------------------------
template <typename T>
IController<T>::~IController()
{
}
//--------------------------------------------------------------------------------
template <typename T>
void IController<T>::SetEntity( T* pEntity )
{
	m_pEntity = pEntity;
}
//--------------------------------------------------------------------------------
template <typename T>
T* IController<T>::GetEntity()
{
	return( m_pEntity );
}
//--------------------------------------------------------------------------------