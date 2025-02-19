#pragma once

#include <stdint.h>

struct typed_obj {};
struct type_member_base : public typed_obj
{
	const char* m_name = 0;
	const char* m_typename = 0;
	size_t m_offset = 0;
};

template<typename _Ty>
struct type_member : public type_member_base
{
	type_member( size_t _offset )
	{
		m_typename = typeid( _Ty ).name();
		m_offset = _offset;
	}
};

template<typename _Ty>
struct type_layout_base
{
	std::vector<type_member_base*> members{};
};

template <typename T1, typename T2>
inline size_t constexpr offset_of( T1 T2::* member ) {
	constexpr T2 object{};
	return size_t( &( object.*member ) ) - size_t( &object );
}

template<typename _Ty, typename _Rty, typename... _Args>
inline size_t constexpr offset_of( _Rty( _Ty::* _member )( _Args... ) ) {
	//constexpr T2 object{};
	//return size_t( &( object.*member ) ) - size_t( &object );
	return (size_t)&_member;
}

template<typename _Ty, auto _Ty::* ..._Members>
struct type_layout : public type_layout_base<_Ty>
{
	type_layout( const std::array<const char*, sizeof...( _Members )>& _names ) {
		members = {
			new type_member<wv::typename_of<decltype( _Members )>::Ty>( offset_of( _Members ) )...
		};

		for( size_t i = 0; i < sizeof...( _Members ); i++ )
			printf( "%s [%zu] %s::%s\n", members[ i ]->m_typename, members[ i ]->m_offset, typeid( _Ty ).name(), _names[ i ] );

	}

private:

};

template<typename _Ty>
struct type_descriptor
{
	type_descriptor( type_layout_base<_Ty> _layout ) {
		
	}

	_Ty* register_ptr( _Ty* _ptr ) {

	}

	void set( const char* _member, void* _value )
	{
		for( type_member_base* m : layout.members )
		{
			if( m->m_name == _member )
				printf( "set %s to something\n", _member );
		}
	}

	std::vector<_Ty*> m_objects;
	type_layout_base<_Ty> layout;
};

