#pragma once

#include <stdint.h>
#include <stdexcept>
#include <set>

namespace arg
{
	template<typename _Kty, typename _Ty>
	class unordered_array
	{
	public:
		unordered_array() { }

		~unordered_array() 
		{
			clear();

			if( m_pBuffer )
				free( m_pBuffer );

			m_pBuffer = nullptr;
			m_size = 0;
		}

		template<typename...Args>
		[[nodiscard]] _Kty emplace( const Args&... _args );
		[[nodiscard]] _Ty& at( const _Kty& _key );

		void erase( const _Kty& _key );
		void clear();
		
		size_t count( void ) { return m_keys.size(); }
		size_t size ( void ) { return m_size; }

	private:

		std::set<_Kty> m_keys;
		uint8_t* m_pBuffer = nullptr;
		size_t m_size = 0;
	};

	template<typename _Kty, typename _Ty>
	template<typename... Args>
	inline _Kty unordered_array<_Kty, _Ty>::emplace( const Args&... _args )
	{
		uint64_t key = 1;
		while( m_keys.find( (_Kty)key ) != m_keys.end() ) // find an unused key
			key++;

		uint64_t index = key - 1; // index is key-1 because key 0 is invalid/none
		size_t endOffset = sizeof( _Ty ) * index + sizeof( _Ty );

		if( endOffset >= m_size )
		{
			uint8_t* newptr = (uint8_t*)realloc( m_pBuffer, endOffset );

			if( newptr == nullptr )
				throw std::runtime_error( "failed to reallocate buffer" );
			
			m_pBuffer = newptr;
			m_size = endOffset;
		}

		_Ty* base = &( (_Ty*)m_pBuffer )[ index ];
		_Ty* obj = new( base ) _Ty( _args... );

		m_keys.insert( (_Kty)key );

		return (_Kty)key;
	}

	template<typename _Kty, typename _Ty>
	inline _Ty& unordered_array<_Kty, _Ty>::at( const _Kty& _key )
	{
		if( m_keys.find( _key ) == m_keys.end() )
			throw std::out_of_range( "Invalid Key" );

		size_t index = _key - 1;

		return ( (_Ty*)m_pBuffer )[ index ];
	}

	template<typename _Kty, typename _Ty>
	inline void unordered_array<_Kty, _Ty>::erase( const _Kty& _key )
	{
		if( m_keys.find( _key ) == m_keys.end() )
			throw std::out_of_range( "Cannot erase Invalid Key" );

		m_keys.erase( _key );

		size_t index = _key - 1;

		_Ty& obj = ( (_Ty*)m_pBuffer )[ index ];
		obj.~_Ty();

		memset( &obj, 0, sizeof( _Ty ) );
	}

	template<typename _Kty, typename _Ty>
	inline void unordered_array<_Kty, _Ty>::clear()
	{
		for( auto& handle : m_keys )
			erase( handle );
		m_keys.clear();
	}

}