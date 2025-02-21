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
#include <atomic>
#include <thread>


#include <wv/type_traits.hpp>
#include "wip/type_descriptor.h"

struct test_struct
{
	uint16_t member_int;
	float member_float;
	char member_char;

	void dothing2() {}
};

typedef type_layout<
		test_struct,
		&test_struct::member_int,
		&test_struct::member_float,
		&test_struct::member_char,
		&test_struct::dothing2> 
	test_struct_layout;
#define test_struct_member_names { "member_int", "member_float", "member_char", "dothing2" }

#include <array>
#include <unordered_map>

namespace wv {

#define INT_BYTES( _i ) _i&0xFF, ((_i>>8)&0xFF), ((_i>>16)&0xFF), ((_i>>24)&0xFF)

#define WBIN_OFFSET_VERSION 4
#define WBIN_OFFSET_NUM_ELEMENT WBIN_OFFSET_VERSION + 4

enum WBType
{
	kInteger32,
	kFloat,
	kDouble,
	kString,
	kBlock
};

struct WBElement
{
	std::string name;
	WBType type;
	size_t sizeBytes;
};


struct WBLayout
{
	WBLayout( std::initializer_list<WBElement> _elements )
	{
		size_t currentOffset = 0;

		for( auto element : _elements )
		{
			if( getOffsetOf( element.name ) != SIZE_MAX )
				continue; // element name already exists

			m_offsets[ element.name ] = currentOffset;

			switch( element.type )
			{
			case kFloat:     currentOffset += sizeof( float );    break;
			case kDouble:    currentOffset += sizeof( double );   break;
			case kInteger32: currentOffset += sizeof( int32_t );  break;
			case kString:    currentOffset += sizeof( uint64_t ); break; // offset into block memory
			case kBlock:     currentOffset += sizeof( uint64_t ); break; // offset into block memory
			}
		}
	}

	size_t getOffsetOf( const std::string& _element ) {
		auto itr = m_offsets.find( _element );
		if( itr == m_offsets.end() )
			return SIZE_MAX;
		return itr->second;
	}

private:
	std::unordered_map<std::string, size_t> m_offsets{};
};

struct WBVersion
{
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
	uint8_t pad;
};

struct WBData
{
	WBData( const wv::WBLayout& _layout ):
		m_layout{ _layout }
	{ }

	void load( uint8_t* _data, size_t _size ) {
		if( m_pBuffer )
			delete[] m_pBuffer;

		m_pBuffer = new uint8_t[ _size ];
		memcpy( m_pBuffer, _data, _size );

		m_version     = _get<WBVersion>( WBIN_OFFSET_VERSION );
		m_numElements = _get<uint32_t>( WBIN_OFFSET_NUM_ELEMENT );
	}

	template<typename _Ty>
	void set( const char* _element, _Ty _value ) {

	}

	template<typename _Ty>
	_Ty get( const char* _element ) {

	}

private:
	size_t _getOffset( const char* _element ) {

	}

	template<typename _Ty>
	_Ty _get( size_t _offset ) {
		_Ty tmp;
		std::memcpy( &tmp, m_pBuffer + _offset, sizeof( _Ty ) );
		return tmp;
	}

	const wv::WBLayout m_layout;

	WBVersion m_version{ 0, 0, 0, 0 };

	uint32_t m_numElements{ 0 };
	uint32_t m_blockOffset{ 0 };

	uint8_t* m_pBuffer{ nullptr };
	size_t m_sizeBuffer{ 0 };
};

}

uint8_t bindata[] = {
	'W', 'B', 'i', 'n',
	1, 4, 2, 1,
	INT_BYTES( 3u ),
	INT_BYTES( UINT32_MAX )
};

struct SerializableObject
{
	std::string name;
	int intValue;
	float fValue;
};

const wv::WBLayout layout{
	{ "name",     wv::kString },
	{ "intValue", wv::kInteger32 },
	{ "fValue",   wv::kFloat }
};











template<size_t _N, typename = void>
struct bits_type;

template<size_t _N> struct bits_type<_N, typename wv::enable_range_t<_N,  0,  8>> { typedef uint8_t  Ty; };
template<size_t _N> struct bits_type<_N, typename wv::enable_range_t<_N,  9, 16>> { typedef uint16_t Ty; };
template<size_t _N> struct bits_type<_N, typename wv::enable_range_t<_N, 17, 32>> { typedef uint32_t Ty; };
template<size_t _N> struct bits_type<_N, typename wv::enable_range_t<_N, 33, 64>> { typedef uint64_t Ty; };

template<size_t _N>
struct bit_register {
	using Ty = typename bits_type<_N>::Ty;
	static constexpr size_t MAX = 1u << _N;

	bit_register() = default;
	bit_register( const Ty& _v ) : m_value{ _v } {}
	bit_register( const bit_register<_N>& _reg ) : m_value{ _reg.m_value } {}
	
	constexpr operator Ty() { return m_value; }

	constexpr void operator=( const size_t& _v ) {
		m_value = _v;
	}

private:
	Ty m_value : _N;
};




using pin = std::atomic_uint8_t;

struct __declspec( novtable ) IBusPins {
	virtual void onSignal() = 0;
};

template<size_t _Size>
struct bus
{
	using Ty = typename bit_register<_Size>::Ty;

	void connect( IBusPins* _buspins ) { 
		pins.push_back( _buspins ); 
		
	}

	void set( Ty _v ) {
		value = _v;
		for( auto& p : pins )
			p->onSignal();
	}

	bit_register<_Size> value;
	std::vector<IBusPins*> pins;
};

template<typename _Ty, size_t _Size>
struct BusPins : IBusPins
{
	using Ty = typename bit_register<_Size>::Ty;

	BusPins( _Ty* _owner ) : m_owner{ _owner } {};

	void connectBusOut( bus<_Size>* _bus ) {
		m_outbus = _bus;
	}

	void connectBusIn( bus<_Size>* _bus ) {
		m_outbus = _bus;
		_bus->connect( this );
	}

	void onSignal() override {
		m_owner->onSignal();
	}

	void set( Ty _v ) {
		m_outbus->set( _v );
	}

	Ty get() { 
		return m_outbus->value; 
	}

private:
	_Ty* m_owner = nullptr;
	bus<_Size>* m_outbus = nullptr;
};


template<size_t _BusSize, size_t _DataSize>
struct ram
{
	using Ty = typename bit_register<_BusSize>::Ty;

	BusPins<ram<_BusSize, _DataSize>, _BusSize> m_inSelect{ this };
	BusPins<ram<_BusSize, _DataSize>, _BusSize> m_outBus{ this };

	void onSignal() {
		m_outBus.set( data[ m_inSelect.get() ]);
	}

	Ty data[ _DataSize ] = { 
		0, 1, 10, 1, 0, 0, 0, 0,
		0, 0,  0, 1, 0, 0, 0, 0
	};
};


struct i4004pins
{
	pin clock1;
	pin clock2;
	pin cmrom;
};

struct i4004
{
	BusPins<i4004, 4> m_inoutData{ this };
	BusPins<i4004, 4> m_outCMRAM{ this };
	
	i4004pins pins;

	void onSignal() {
		printf( "" );
	}

	void cycle() {
		auto v = m_inoutData.get();
		m_outCMRAM.set( v + 1 );
	}
};

i4004 gCPU;
ram<4, 16> gRAM;
bus<4> gCMRAM;
bus<4> gDataBus;

int main()
{
	gCPU.m_outCMRAM.connectBusOut( &gCMRAM ); // connect cpu cram pins to select bus
	gRAM.m_outBus.connectBusOut( &gDataBus ); // connect ram out to data bus

	gCPU.m_inoutData.connectBusIn( &gDataBus ); // connect data bus to cpu data pins
	gRAM.m_inSelect.connectBusIn( &gCMRAM );    // connect ram select bus to ram select pins

	while( true )
	{
		printf( "%i ", (int)gDataBus.value );
		gCPU.cycle();
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
	
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

	wv::WBData data{ layout };
	data.load( bindata, sizeof( bindata ) );

	return 0;
}
