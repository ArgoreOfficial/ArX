#include <stdio.h>

#include "test.hpp"
#include "tests/test_array_view.hpp"
#include "tests/test_ptr_reloc.hpp"
#include "tests/test_reflected_function.hpp"
#include "tests/test_registry.hpp"
#include "tests/test_strong_type.hpp"
#include "tests/test_unordered_array.hpp"

#include <stdint.h>

#include <typeinfo>
#include <vector>
#include <array>
#include <string>

#include <arx/typename_of.hpp>

struct test_struct
{
	uint16_t member_int;
	float member_float;
	char member_char;

	void dothing2() {}
};

struct typed_obj {};
struct type_member_base : public typed_obj { 
	const char* m_name = 0;
	const char* m_typename = 0;
	size_t m_offset = 0;
};

template<typename _Ty>
struct type_member : public type_member_base { 
	type_member( size_t _offset )
	{
		m_typename = typeid( _Ty ).name();
		m_offset = _offset;
	}
};

template<typename _Ty>
struct type_layout_base {
	std::vector<type_member_base*> members{};
};

template <typename T1, typename T2>
inline size_t constexpr offset_of( T1 T2::* member ) {
	constexpr T2 object{};
	return size_t( &( object.*member ) ) - size_t( &object );
}

template<typename _Ty, typename _Rty, typename... _Args>
inline size_t constexpr offset_of( _Rty( _Ty::*_member )( _Args... ) ) {
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
			printf( "%s [%zu] %s::%s\n", members[ i ]->m_typename, members[ i ]->m_offset, typeid( _Ty ).name(), _names[ i]  );

	}

private:

};

template<typename _Ty>
struct type_descriptor
{
	type_descriptor( type_layout_base<_Ty> _layout ) { }

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

typedef type_layout<
		test_struct,
		&test_struct::member_int,
		&test_struct::member_float,
		&test_struct::member_char,
		&test_struct::dothing2> 
	test_struct_layout;
#define test_struct_member_names { "member_int", "member_float", "member_char", "dothing2" }

/////////////////////////////////////////////////////////////////
//// ArXFX Brainstorming                                     ////
/////////////////////////////////////////////////////////////////

#include <arx/strong_type.hpp>
#define ARXFX_BACKEND_VK

namespace wv {

// arxfx/core/
struct pipeline_interface {
	std::string name;
};

// arxfx/vk/
struct pipeline_vk : pipeline_interface {
	/* VkPipeline m_pipeline; */
};

typedef strong_type<uint16_t, struct _PipelineID> pipeline_id;

#ifdef ARXFX_BACKEND_VK
typedef pipeline_vk pipeline;
#elif defined( ARXFX_BACKEND_GL )
typedef pipeline_gl pipeline;
#elif defined( ARXFX_BACKEND_MTL )
typedef pipeline_mtl pipeline;
#else
#error no backend defined
#endif

}

/////////////////////////////////////////////////////////////////

template<typename _Ty> struct shader;
template<> struct shader<struct base> { 
	void basefunc() { printf( "test\n" ); }
	int someBase; 
};

template<> 
struct shader<struct vk> : shader<struct base>
{
	void print() { printf( "vk shader\n" ); }
	int someVkThing;
};

template<>
struct shader<struct gl> : shader<struct base>
{
	void print() { printf( "gl shader\n" ); }
	int someGlThing;
};

template<typename _Ty>
struct gfx_device
{
	shader<_Ty> m_shader{};
};

/////////////////////////////////////////////////////////////////

struct someObject {
	float value = 0.0f;
};

struct cAssetManager
{

	template<typename _Ty, typename ... _Args>
	static _Ty* load( const std::string& _path, _Args... _args );

	template<typename _Ty, typename ... _Args>
	static _Ty* load( uint8_t* _pData, size_t _sizeData, _Args... _args );
};

template<>
static someObject* cAssetManager::load<someObject, float>( const std::string& _path, float _value )
{
	someObject* s = new someObject();
	s->value = _value;
	return s;
}

template<>
static someObject* cAssetManager::load<someObject, float>( uint8_t* _pData, size_t _sizeData, float _value )
{
	someObject* s = new someObject();
	s->value = _value;
	return s;
}




template<typename _Oty, typename _Ty>
struct public_read
{
	public_read() = default;
	public_read( _Ty _v ) : m_value{ _v } {}

private:
	friend _Oty;
	void operator=( _Ty _v ) { m_value = _v; }

	_Ty m_value;
};

class tester
{
public:
	tester()
	{
		bobber = 2;
	}

	public_read<tester, int> bobber{ 3 };

};



int main()
{
	wv::test_array_view();
	wv::test_ptr_reloc();
	wv::test_reflected_function();
	wv::test_registry();
	wv::test_strong_type();
	wv::test_unordered_array();

	printf( " ::------ type_layout test ------::\n" );
	
	test_struct coolthing;

	type_descriptor<test_struct> test_struct_set{ 
		test_struct_layout{ test_struct_member_names } 
	};


	test_struct_set.set( "member_int", 0 );

	printf( " ::------------------------------::\n\n" );

	gfx_device<struct vk> vkDevice{};
	gfx_device<struct gl> glDevice{};

	vkDevice.m_shader.basefunc();
	vkDevice.m_shader.print();

	glDevice.m_shader.basefunc();
	glDevice.m_shader.print();
	
	// ignore this
	// load object from file                                 path                   user-variable
	someObject* theObject = cAssetManager::load<someObject>( "file/someobject.ini", 1.0f );
	// load object from data
	uint8_t dat[] = { 0, 1, 23, 12, 3, 123, 1 };
	someObject* theOtherObject = cAssetManager::load<someObject>( dat, sizeof( dat ), 1.0f);



	tester asd2;
	//asd2.bobber = 3;


	return 0;
}
